#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <error.h>
#include <mysql/mysql.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <list>
#include <string>

#include "../lock/locker.h"
#include "../log/log.h"

class connection_pool {
   public:
    MYSQL *GetConnection();               // 获取数据库连接
    bool ReleaseConnection(MYSQL *conn);  // 释放连接
    int GetFreeConn();                    // 获取连接
    void DestroyPool();                   // 销毁所有连接

    // 单例模式
    static connection_pool *GetInstance();
    // static connection_pool& Instance() {
    //     static connection_pool my_instance;
    //     return my_instance;
    // }

    // connection_pool(connection_pool const&) = delete;
    // connection_pool(connection_pool&&) = delete;
    // connection_pool& operator=(connection_pool const&) = delete;
    // connection_pool& operator=(connection_pool&&) = delete; 

    void init(const std::string &url, const std::string &user,
              const std::string &password, const std::string &db_name,
              const int &port, const int &max_conn, const bool &close_log);

    std::string m_url;       // 主机地址
    std::string m_port;      // 数据库端口号
    std::string m_user;      // 登陆数据库用户名
    std::string m_password;  // 登陆数据库密码
    std::string m_db_name;   // 使用数据库名
    bool m_close_log;        // 日志开关

   protected:
    connection_pool();
    ~connection_pool();

  private:
    int m_max_conn;   // 最大连接数
    int m_cur_conn;   // 当前已使用的连接数
    int m_free_conn;  // 当前空闲的连接数
    locker lock;
    std::list<MYSQL *> conn_list;  // 连接池
    sem reserve;
};

class connectionRAII {
   public:
    connectionRAII(MYSQL **con, connection_pool *connPool);
    ~connectionRAII();

   private:
    MYSQL *conRAII;
    connection_pool *poolRAII;
};

#endif
