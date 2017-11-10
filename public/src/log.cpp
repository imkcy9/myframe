/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "log.h"

#ifdef USE_SPDLOG

bool LOGGER::init(const char* app_name, bool is_daemon, int level, const char* log_path) {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%F][thread %t][%n %l] %v");
    if (!is_daemon) {
        spdlog::stdout_color_mt("Log");

    } else {
        try {
            spdlog::daily_logger_mt("Log", log_path, 2, 30);
        } catch (spdlog::spdlog_ex& e) {
            spdlog::daily_logger_mt("Log", "log", 2, 30);
        }
    }
    spdlog::get("Log")->flush_on(spdlog::level::warn);
    spdlog::set_level((spdlog::level::level_enum)level);
    return true;
}

void LOGGER::dropall() {
    spdlog::drop_all();
}
#endif
#ifdef USE_YTLOG

bool LOGGER::init(const char* app_name, bool is_daemon, int level, const char* log_path) {
    static yt::OStreamLogger ostreamLogger(std::cout);
    AC_SET_DEFAULT_LOGGER(&ostreamLogger);
    
    if(!is_daemon)
        return true;
    //设置log信息
    const int logPriority = 1 << level;
    const std::string strLogDirAndPrefix = std::string(log_path) + app_name;
    switch (logPriority) {
#ifdef AC_HAS_TRACE
        case yt::LP_TRACE: // 7
            static yt::DateFileLogger tracelog(strLogDirAndPrefix + ".trace");
            AC_SET_LOGGER(yt::LP_TRACE, &tracelog);
            // break;	// 刻意进行注释
#endif // AC_HAS_TRACE
#ifdef AC_HAS_DEBUG
        case yt::LP_DEBUG: // 8
            static yt::DateFileLogger debuglog(strLogDirAndPrefix + ".debug");
            AC_SET_LOGGER(yt::LP_DEBUG, &debuglog);
            // break;	// 刻意进行注释
#endif // AC_HAS_DEBUG
#ifdef AC_HAS_INFO
        case yt::LP_INFO: // 9
            static yt::DateFileLogger infolog(strLogDirAndPrefix + ".sys");
            AC_SET_LOGGER(yt::LP_INFO, &infolog);
            // break;	// 刻意进行注释
#endif // AC_HAS_INFO
        case yt::LP_USER1: // 10
            static yt::DateFileLogger runlog(strLogDirAndPrefix + ".run");
            AC_SET_LOGGER(yt::LP_USER1, &runlog);
            // break;	// 刻意进行注释
        case yt::LP_USER2: // 11
            static yt::DateFileLogger run2log(strLogDirAndPrefix + ".run2");
            AC_SET_LOGGER(yt::LP_USER2, &run2log);
            // break;	// 刻意进行注释
#ifdef AC_HAS_WARNING
        case yt::LP_WARNING: // 12
            static yt::DateFileLogger warnlog(strLogDirAndPrefix + ".warn");
            AC_SET_LOGGER(yt::LP_WARNING, &warnlog);
            // break;	// 刻意进行注释
#endif // AC_HAS_WARNING
#ifdef AC_HAS_ERROR
        case yt::LP_ERROR: // 13
            static yt::DateFileLogger errlog(strLogDirAndPrefix + ".error");
            AC_SET_LOGGER(yt::LP_ERROR, &errlog);
            // break;	// 刻意进行注释
#endif // AC_HAS_ERROR
#ifdef AC_HAS_CRITICAL
        case yt::LP_CRITICAL: // 14
            static yt::DateFileLogger criticalLog(strLogDirAndPrefix + ".critical");
            AC_SET_LOGGER(yt::LP_CRITICAL, &criticalLog);
            // break;	// 刻意进行注释
#endif // AC_HAS_CRITICAL
            AC_INFO("Set Log File Success, logPriority(%d)!", level);
            return true;
            break;
        default:
            AC_ERROR("Set Log File Failed, logPriority(%d) Is Not Exists!", level);
            return false;
            break;
    }
    return false;
}

void LOGGER::dropall() {

}
#endif
#ifdef USE_GLOG
#include <string>
#include <fstream>

void SignalHandle(const char* data, int size) {

    std::ofstream fs("dump.log", std::ios::app);

    std::string str = std::string(data, size);

    fs << str;

    fs.close();

    LOG(ERROR) << str;

}

void LOGGER::init(const char* app_name, bool is_daemon, int level, const char* log_path) {
    google::InitGoogleLogging(app_name);

    if (!is_daemon) {
        FLAGS_alsologtostderr = true;
        FLAGS_colorlogtostderr = true;
    } else {
        FLAGS_log_dir = log_path;
        FLAGS_max_log_size = 10;
    }
    FLAGS_logbuflevel = level;

    //google::InstallFailureSignalHandler();
    //默认捕捉 SIGSEGV 信号信息输出会输出到stderr，可以通过下面的方法自定义输出方式：
    //google::InstallFailureWriter(&SignalHandle);
}

void LOGGER::dropall() {
    google::ShutdownGoogleLogging();
}
#endif
