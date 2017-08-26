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
#include "../../public/include/tinyxml2.h"
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
        return err == XMLError::XML_SUCCESS;
    }
    //add defines here 
    DECLARE_CONFIG_GETSTR(config,basic,logdir)
    DECLARE_CONFIG_GETINT(config,basic,logPriority)

private:

    config() {
    }

    virtual ~config() {
    }
    std::string file_name;
    XMLDocument doc;
};


#endif /* CONFIG_H */

