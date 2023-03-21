#include "sql_connection_pool.h"

#include <mysql/mysql.h>
#include <pthread.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <string>

connection_pool::connection_pool() : m_cur_conn(0), m_free_conn(0) {}

connection_pool *connection_pool::GetInstance() {
    static connection_pool connPool;
    return &connPool;
}

// 构造初始化
void connection_pool::init(const std::string &url, const std::string &user,
                           const std::string &password,
                           const std::string &db_name, const int &port,
                           const int &max_conn, const bool &close_log) {
    m_url = url;
    m_port = port;
    m_user = user;
    m_password = password;
    m_db_name = db_name;
    m_close_log = close_log;

    for (int i = 0; i < max_conn; i++) {
        MYSQL *con = nullptr;
        con = mysql_init(con);

        if (con == nullptr) {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        con =
            mysql_real_connect(con, url.c_str(), user.c_str(), password.c_str(),
                               db_name.c_str(), port, nullptr, 0);

        if (con == nullptr) {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        conn_list.push_back(con);
        ++m_free_conn;
    }

    reserve = sem(m_free_conn);

    m_max_conn = m_free_conn;
}

// 当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL *connection_pool::GetConnection() {
    MYSQL *con = nullptr;

    if (0 == conn_list.size()) return nullptr;

    reserve.wait();

    lock.lock();

    con = conn_list.front();
    conn_list.pop_front();

    --m_free_conn;
    ++m_cur_conn;

    lock.unlock();
    return con;
}

// 释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con) {
    if (nullptr == con) return false;

    lock.lock();

    conn_list.push_back(con);
    ++m_free_conn;
    --m_cur_conn;

    lock.unlock();

    reserve.post();
    return true;
}

// 销毁数据库连接池
void connection_pool::DestroyPool() {
    lock.lock();
    if (conn_list.size() > 0) {
        std::list<MYSQL *>::iterator it;
        for (it = conn_list.begin(); it != conn_list.end(); ++it) {
            MYSQL *con = *it;
            mysql_close(con);
        }
        m_cur_conn = 0;
        m_free_conn = 0;
        conn_list.clear();
    }

    lock.unlock();
}

// 当前空闲的连接数
int connection_pool::GetFreeConn() { return this->m_free_conn; }

connection_pool::~connection_pool() { DestroyPool(); }

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool) {
    *SQL = connPool->GetConnection();

    conRAII = *SQL;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII() { poolRAII->ReleaseConnection(conRAII); }