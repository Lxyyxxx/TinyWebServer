#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include "./http/http_conn.h"
#include "./threadpool/threadpool.h"

const int MAX_FD = 0x10000;  // 最大文件描述符 `ulimit -n` = 0x100000
const int MAX_EVENT_NUMBER = 10000;  // 最大事件数
const int TIMESLOT = 5;              // 最小超时单位

class WebServer {
   public:
    WebServer() = delete;  // 取消默认构造函数
    ~WebServer();

    WebServer(const int &port,              // 端口
              const std::string &user,      // 用户名
              const std::string &password,  // 密码
              const std::string &db_name,   // 数据库
              const bool &log_write,        // 日志写入方式
              const bool &opt_linger,       // 优雅关闭链接
              const int &trig_mode,         // 触发组合模式
              const int &sql_num,           // 数据库连接池数量
              const int &thread_num,        // 线程池内的线程数量
              const bool &close_log,        // 是否关闭日志
              const bool &actor_model       // 并发模型选择
    );

    WebServer(const WebServer &ws) = delete;  // 取消复制构造函数

    void threadPool();
    void sqlPool();
    void logWrite();
    void trigMode();
    void eventListen();
    void eventLoop();
    void timer(const int &connfd, const struct sockaddr_in &client_address);
    void adjustTimer(util_timer *timer);
    void dealTimer(util_timer *timer, const int &sockfd);
    bool dealClinetData();
    bool dealWithSignal(bool &timeout, bool &stop_server);
    void dealWithRead(const int &sockfd);
    void dealWithWrite(const int &sockfd);

   private:
    // 基础
    int m_port;
    char *m_root;
    bool m_log_write;
    bool m_close_log;
    bool m_actor_model;

    int m_pipefd[2];
    int m_epollfd;
    http_conn *users;

    // 数据库相关
    connection_pool *m_conn_pool;
    std::string m_user;      // 登陆数据库用户名
    std::string m_password;  // 登陆数据库密码
    std::string m_db_name;   // 使用数据库名
    int m_sql_num;

    // 线程池相关
    threadpool<http_conn> *m_pool;
    int m_thread_num;

    // epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER];

    int m_listenfd;
    bool m_OPT_LINGER;
    int m_TRIGMode;
    bool m_LISTENTrigmode;  // listenfd 触发模式
    bool m_CONNTrigmode;    // connfd 触发模式

    // 定时器相关
    client_data *users_timer;
    Utils utils;
};
#endif
