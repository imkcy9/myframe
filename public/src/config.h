/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   config.h
 * Author: chengyeke
 *
 * Created on 2017年8月25日, 上午11:27
 */

#ifndef CONFIG_H
#define CONFIG_H
#include "tinyxml2.h"
#include <string>

#define DECLARE_CONFIG_GETSTR(rootnode,childnode,element) inline const char* get_##childnode##_##element(){\
            return doc.FirstChildElement(#rootnode)->FirstChildElement(#childnode)->Attribute(#element);\
            }\
            
#define DECLARE_CONFIG_GETBOOL(rootnode,childnode,element) bool get_##childnode##_##element(){\
            return doc.FirstChildElement(#rootnode)->FirstChildElement(#childnode)->BoolAttribute(#element);\
            }\
            
#define DECLARE_CONFIG_GETINT(rootnode,childnode,element) int get_##childnode##_##element(){\
            return doc.FirstChildElement(#rootnode)->FirstChildElement(#childnode)->IntAttribute(#element);\
            }\

#define DECLARE_CONFIG_GETINT64(rootnode,childnode,element) int get_##childnode##_##element(){\
            return doc.FirstChildElement(#rootnode)->FirstChildElement(#childnode)->Int64Attribute(#element);\
            }\
            
#define DECLARE_CONFIG_GETFLOAT(rootnode,childnode,element) int get_##childnode##_##element(){\
            return doc.FirstChildElement(#rootnode)->FirstChildElement(#childnode)->FloatAttribute(#element);\
            }\
            
using namespace tinyxml2;

class config {
public:

    static config* Instance() {
        static config sconf;
        return &sconf;
    }

    inline bool load_file(const char* filename) {
        file_name = filename;
        XMLError err = doc.LoadFile(filename);
        doc.PrintError();
        return err == XMLError::XML_SUCCESS;
    }
    //add defines here 
    DECLARE_CONFIG_GETSTR(config,basic,logfile)
    DECLARE_CONFIG_GETINT(config,basic,logPriority)
    DECLARE_CONFIG_GETSTR(config,basic,logprefix)
            
    DECLARE_CONFIG_GETSTR(config,mysqlconn,dbip)
    DECLARE_CONFIG_GETSTR(config,mysqlconn,username)
    DECLARE_CONFIG_GETSTR(config,mysqlconn,passwd)
    DECLARE_CONFIG_GETSTR(config,mysqlconn,dbname)
    
    DECLARE_CONFIG_GETSTR(config,mdconfig,connect_addr);
    DECLARE_CONFIG_GETSTR(config,mdconfig,bind_addr);
    DECLARE_CONFIG_GETSTR(config,mdconfig,clear_signal_time);
    DECLARE_CONFIG_GETSTR(config,mdconfig,storage_path);
    DECLARE_CONFIG_GETBOOL(config,mdconfig,is_storage);
    DECLARE_CONFIG_GETBOOL(config,mdconfig,update_innercode_from_sina_before_subscribe);
    DECLARE_CONFIG_GETBOOL(config,mdconfig,tick_format);
    
    DECLARE_CONFIG_GETSTR(config,zookeeper,zookeeper_serverip);
    DECLARE_CONFIG_GETSTR(config,zookeeper,zookeeper_register_value);

private:

    config() {
    }

    virtual ~config() {
        doc.Clear();
    }
    std::string file_name;
    XMLDocument doc;
};


#endif /* CONFIG_H */

