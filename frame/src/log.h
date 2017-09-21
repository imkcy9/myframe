/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   log.h
 * Author: chengyeke
 *
 * Created on 2017年8月25日, 下午1:03
 */

#ifndef LOG_H
#define LOG_H
#include "spdlog/spdlog.h"

#ifdef SPDLOG_TRACE_ON
#define LOG_TRACE(...) spdlog::get("Log")->trace("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_DEBUG(...) spdlog::get("Log")->debug("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_INFO(...) spdlog::get("Log")->info("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_WARN(...) spdlog::get("Log")->warn("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_ERROR(...) spdlog::get("Log")->error("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::get("Log")->critical("[" __FILE__ " line #" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#else
#define LOG_TRACE(...) spdlog::get("Log")->trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::get("Log")->debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::get("Log")->info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::get("Log")->warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::get("Log")->error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::get("Log")->critical(__VA_ARGS__)
#endif
namespace LOGGER {
    void init(bool is_daemon,int level,const char* log_path);
}

#endif /* LOG_H */

