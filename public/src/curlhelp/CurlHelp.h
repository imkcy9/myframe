/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CurlHelp.h
 * Author: chengye.ke
 *
 * Created on 2017年6月22日, 下午1:27
 */

#ifndef CURLHELP_H
#define CURLHELP_H
#include <curl/curl.h>
#include <string>

class CurlHelp {
public:
    CurlHelp();
    virtual ~CurlHelp();
    void perform_to_string(const char* url);
    void set_string(std::string& str);
    void set_string(char* str);
    bool get_next_line(std::string& out_string);
private:
    std::string m_curlString;
    std::string::size_type m_lastPos;
};

#endif /* CURLHELP_H */

