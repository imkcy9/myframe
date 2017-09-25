/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   util.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月20日, 上午10:16
 */

#include "util.h"
#include <string.h>

/**
 * 获取字符串时间
 * @param strformat
 * @return 
 */
std::string timeutil::get_strtime(time_t timetick, const char* strformat) {
    struct tm tmInfo;
    localtime_r(&timetick,&tmInfo);
    char buffer[256];
    strftime(buffer,sizeof(buffer),strformat,&tmInfo);
    return buffer;
}

/**
 * 字符串时间转换成time_t
 * @param strtime 字符串时间
 * @param strformat 格式如："%Y-%m-%d %H:%M:%S"
 * @return 
 */
time_t timeutil::strtime_to_time_t(const char* strtime, const char* strformat) {
    time_t timeValue = 0;
    if (NULL != strtime && 0 < strlen(strtime)) {
        struct tm tmInfo;
        memset(&tmInfo, 0, sizeof (tmInfo));
        if (NULL != strptime(strtime, strformat, &tmInfo)) {
            timeValue = mktime(&tmInfo);
        }
    }
    return timeValue;
}

time_t timeutil::get_HMS_diff_time(const char* totime) {
    std::string date = timeutil::get_strtime(time(0),"%Y-%m-%d");
    time_t totime_t = timeutil::strtime_to_time_t(date.append(" ").append(totime).c_str() ,"%Y-%m-%d %H:%M:%S");
    time_t current_time_t = time(0);
    if(current_time_t < totime_t) {
        return totime_t - current_time_t;
    } else {
        return ((totime_t + 3600*24) - current_time_t);
    }
}


