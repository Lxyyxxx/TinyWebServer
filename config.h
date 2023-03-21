#ifndef CONFIG_H
#define CONFIG_H

#include "webserver.h"

class Config {
   public:
    Config() = delete;
    Config(const Config &c) = delete;
    Config(int argc, char *argv[]);

    int PORT;          // 端口号
    bool LOGWrite;     // 日志写入方式
    int TRIGMode;      // 触发组合模式
    bool OPT_LINGER;   // 优雅关闭连接
    int sql_num;       // 数据库连接池数量
    int thread_num;    // 线程池内的线程数量
    bool close_log;    // 是否关闭日志
    bool actor_model;  // 并发模型选择
};

#endif