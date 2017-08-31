/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "log.h"

void LOGGER::init(bool is_daemon, int level, const char* log_path) {
    if (!is_daemon) {
        spdlog::stdout_color_mt("Log");

    } else {
        spdlog::daily_logger_mt("Log", log_path, 2, 30);
    }
    spdlog::set_level((spdlog::level::level_enum)level);

}
