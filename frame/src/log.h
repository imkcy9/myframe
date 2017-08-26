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

#define LOG_TRACE(...) spdlog::get("Log")->trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::get("Log")->debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::get("Log")->info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::get("Log")->warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::get("Log")->error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::get("Log")->critical(__VA_ARGS__)
namespace LOGGER {
    void init(bool is_daemon,int level,const char* log_path) {
        if(!is_daemon) {
            spdlog::stdout_color_mt("Log");
            
        } else {
            spdlog::daily_logger_mt("Log",log_path, 2, 30);
        }
        spdlog::set_level((spdlog::level::level_enum)level);
        
    }
}

#endif /* LOG_H */

