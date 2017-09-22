/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CurlHelp.cpp
 * Author: chengye.ke
 * 
 * Created on 2017年6月22日, 下午1:27
 */

#include "CurlHelp.h"
#include <string>
#include <iostream>

CurlHelp::CurlHelp() : m_lastPos(std::string::npos){
}

CurlHelp::~CurlHelp() {
}

size_t write_string(void *ptr, size_t size, size_t nmemb, void *stream) {
    //size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
    //char* p = (char*) stream;
    std::string* str = (std::string*)stream;
    str->append((const char*)ptr,nmemb);
    return size * nmemb;
}

void CurlHelp::perform_to_string(const char* url) {
    CURL *curl = 0;
    m_curlString.clear();
    m_lastPos = std::string::npos;
    CURLcode res;
    std::string strYahooURL = url;

    curl = curl_easy_init();

    if (curl) {


        CURLcode err = curl_easy_setopt(curl, CURLOPT_URL, strYahooURL.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        err = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string);
        //err = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) Buffer);
        err = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &m_curlString);
        err = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
        err = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);
        err = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        res = curl_easy_perform(curl);
        /* always cleanup */

        size_t size = m_curlString.size();
        if(m_curlString[size] == '\0')
        {
            //把结尾换成\n便于字符串分割
            m_curlString.replace(size,1,"\n");
        }
        //std::string::size_type pos = m_curlString.find_last_of('\n');
        //std::cout << m_curlString << std::endl;
        curl_easy_cleanup(curl);
    }
}

void CurlHelp::set_string(std::string& str) {
    m_curlString = str;
}

void CurlHelp::set_string(char* str) {
    m_curlString = str;
}


bool CurlHelp::get_next_line(std::string& out_string) {
    std::string::size_type pos = m_curlString.find('\n',m_lastPos+1);
    if(pos == std::string::npos || pos == m_lastPos) {
        return false;
    }
    out_string = m_curlString.substr(m_lastPos + 1, pos - m_lastPos-1);

    m_lastPos = pos;

    return true;
}
