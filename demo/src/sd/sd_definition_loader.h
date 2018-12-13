/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sd_definition_loader.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午5:16
 */

#ifndef SD_DEFINITION_LOADER_H
#define SD_DEFINITION_LOADER_H
#include <string>
#include <unordered_map>
#include "service_definitions.h"
#include "data/field_enum_type.h"
#include "generated-sources/message_type.h"
namespace kt {
class sd_definition_loader {
public:
    sd_definition_loader();
    sd_definition_loader(const sd_definition_loader& orig);
    virtual ~sd_definition_loader();

    static bool init();
    
    static int get_field_tag(std::string tag_name);
    static field_enum_type_t get_field_type(int tag);
    static std::string get_field_name(int tag);
private:
    static std::string ORC_MESSAGES_PREFIX;
    static std::string ORC_SERVICES_PREFIX;
    static std::unordered_map<int, kt::message_type_t> _orc_messages;
    static std::unordered_map<int, kt::service_type_t> _orc_services;
    static std::unordered_map<int, kt::field_type_t> _fields;
    static std::unordered_map<std::string, int> _field_tags;
    //static std::unordered_map<std::string, kt::field_enum_type_t> _types;
    static std::unordered_map<std::string, std::string> _types;
    static void process_namespace(service_definitions::xml_namespace);
    
};
}
#endif /* SD_DEFINITION_LOADER_H */

