/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   util.h
 * Author: chengyeke
 *
 * Created on 2017年9月20日, 上午10:16
 */

#ifndef UTIL_H
#define UTIL_H
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <string>


//specifier	Replaced by	Example
//%a	Abbreviated weekday name *	Thu
//%A	Full weekday name *	Thursday
//%b	Abbreviated month name *	Aug
//%B	Full month name *	August
//%c	Date and time representation *	Thu Aug 23 14:55:02 2001
//%d	Day of the month (01-31)	23
//%H	Hour in 24h format (00-23)	14
//%I	Hour in 12h format (01-12)	02
//%j	Day of the year (001-366)	235
//%m	Month as a decimal number (01-12)	08
//%M	Minute (00-59)	55
//%p	AM or PM designation	PM
//%S	Second (00-61)	02
//%U	Week number with the first Sunday as the first day of week one (00-53)	33
//%w	Weekday as a decimal number with Sunday as 0 (0-6)	4
//%W	Week number with the first Monday as the first day of week one (00-53)	34
//%x	Date representation *	08/23/01
//%X	Time representation *	14:55:02
//%y	Year, last two digits (00-99)	01
//%Y	Year	2001
//%Z	Timezone name or abbreviation	CDT
//%%	A % sign	%

namespace timeutil {
    /**
     * 获取字符串时间
     * @param strformat
     * @return 
     */
    std::string get_strtime(time_t timetick = time(0), const char* strformat = "%H:%M:%S");
    /**
     * 字符串时间转换成time_t
     * @param strtime 字符串时间
     * @param strformat 字符串格式如："%Y-%m-%d %H:%M:%S"
     * @return 
     */
    time_t strtime_to_time_t(const char* strtime, const char* strformat);
    /**
     * 获取从当前时间走到下一个totime需要的时间  ,时间格式"%H:%M:%S"
     * @param totime
     * @return 
     */
    time_t get_HMS_diff_time( const char* totime);
    
}


#endif /* UTIL_H */

