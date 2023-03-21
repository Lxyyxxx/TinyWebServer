#include "config.h"

int main(int argc, char *argv[]) {
    // 需要修改的数据库信息：登录名,密码,库名
    std::string user = "root";
    std::string passwd = "mypasswd";
    std::string db_name = "tinywebserverdb";

    // 命令行解析参数，更新为构造函数初始化
    Config config{argc, argv};

    // 更新为构造函数初始化
    WebServer server{config.PORT,
                     user,
                     passwd,
                     db_name,
                     config.LOGWrite,
                     config.OPT_LINGER,
                     config.TRIGMode,
                     config.sql_num,
                     config.thread_num,
                     config.close_log,
                     config.actor_model};

    server.logWrite();     // 日志
    server.sqlPool();      // 数据库
    server.threadPool();   // 线程池
    server.trigMode();     // 触发模式
    server.eventListen();  // 监听事件
    server.eventLoop();    // 运行

    return 0;
}