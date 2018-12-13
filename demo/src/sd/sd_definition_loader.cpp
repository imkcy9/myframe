/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sd_definition_loader.cpp
 * Author: kecy
 * 
 * Created on 2018年11月15日, 下午5:16
 */

#include <algorithm>
#include <unordered_map>
#include "sd_definition_loader.h"
#include "service_definitions.h"
#include "data/sd_constants.h"

kt::sd_definition_loader::sd_definition_loader() {

}

kt::sd_definition_loader::sd_definition_loader(const sd_definition_loader& orig) {
}

kt::sd_definition_loader::~sd_definition_loader() {
}

std::string kt::sd_definition_loader::ORC_MESSAGES_PREFIX = "orc.messages.";
std::string kt::sd_definition_loader::ORC_SERVICES_PREFIX = "orc.services.";
std::unordered_map<int, kt::message_type_t> kt::sd_definition_loader::_orc_messages;
std::unordered_map<int, kt::service_type_t> kt::sd_definition_loader::_orc_services;
std::unordered_map<int, kt::field_type_t> kt::sd_definition_loader::_fields;
std::unordered_map<std::string, int> kt::sd_definition_loader::_field_tags;
//std::unordered_map<std::string, kt::field_enum_type_t> kt::sd_definition_loader::_types;
std::unordered_map<std::string, std::string> kt::sd_definition_loader::_types;

bool kt::sd_definition_loader::init() {
    service_definitions service;
    if(!service.load_resource("./resources/service-definitions.xml"))
        return false;

    for (service_definitions::xml_namespace& namespace_ : service.getNamespaces()) {
        process_namespace(namespace_);
    }
    
    service_definitions infra;
    if(!infra.load_resource("./resources/infrastructure-message-definitions.xml"))
        return false;
    
    for (service_definitions::xml_namespace& namespace_ : infra.getNamespaces()) {
        process_namespace(namespace_);
    }
    
    service_definitions market;
    if(!infra.load_resource("./resources/market-specific-definitions.xml"))
        return false;
    
    for (service_definitions::xml_namespace& namespace_ : market.getNamespaces()) {
        process_namespace(namespace_);
    }
    
    kt::sd_constants_t::fields.init();
    
    return true;
}

int kt::sd_definition_loader::get_field_tag(std::string tag_name) {
    auto it = _field_tags.find(tag_name);
    if (it != _field_tags.end())
        return it->second;
    return -1;
}

#define IF_AND_RETURN(src_string_type, dec_string_type, return_enum_type) if(src_string_type == dec_string_type) return return_enum_type

kt::field_enum_type_t kt::sd_definition_loader::get_field_type(int tag) {
    std::string type;
    auto it = _fields.find(tag);
    if (it != _fields.end()) {
        kt::field_type_t& field_type = it->second;
        type = field_type.GetType();
        if(field_type.IsArray()) {
            return field_enum_type_t::ARRAY;
        }
        if (type.find("Enum") != std::string::npos) {
            //todo;
            //return field_enum_type_t::INT8;
            auto ty = _types.find(type);
            if(ty != _types.end()) {
                type = ty->second;
            }
        }
        std::transform(type.begin(), type.end(), type.begin(), ::toupper);
        IF_AND_RETURN(type, "INT8", field_enum_type_t::INT8);
        IF_AND_RETURN(type, "INT16", field_enum_type_t::INT16);
        IF_AND_RETURN(type, "INT32", field_enum_type_t::INT32);
        IF_AND_RETURN(type, "INT64", field_enum_type_t::INT64);
        IF_AND_RETURN(type, "UINT32", field_enum_type_t::UINT32);
        IF_AND_RETURN(type, "UINT64", field_enum_type_t::UINT64);
        IF_AND_RETURN(type, "STRING", field_enum_type_t::STRING);
        IF_AND_RETURN(type, "UINT16", field_enum_type_t::UINT16);
        IF_AND_RETURN(type, "CHAR", field_enum_type_t::CHAR);
        IF_AND_RETURN(type, "STRUCT", field_enum_type_t::STRUCT);
        IF_AND_RETURN(type, "ARRAY", field_enum_type_t::ARRAY);
        IF_AND_RETURN(type, "REAL64", field_enum_type_t::REAL64);
        IF_AND_RETURN(type, "REAL32", field_enum_type_t::REAL32);
        IF_AND_RETURN(type, "BOOLEAN", field_enum_type_t::BOOLEAN);
        IF_AND_RETURN(type, "DATE", field_enum_type_t::DATE);
        IF_AND_RETURN(type, "TIME", field_enum_type_t::TIME);
        IF_AND_RETURN(type, "TIMESTAMP", field_enum_type_t::TIMESTAMP);
        IF_AND_RETURN(type, "DATETIME", field_enum_type_t::DATETIME);
        IF_AND_RETURN(type, "OCTETS", field_enum_type_t::OCTETS);
        IF_AND_RETURN(type, "UNKNOWN", field_enum_type_t::UNKNOWN);

        return field_enum_type_t::STRUCT;
    }
    return field_enum_type_t::UNKNOWN;
}

std::string kt::sd_definition_loader::get_field_name(int tag) {
    auto it = _fields.find(tag);
    if (it != _fields.end()) {
        kt::field_type_t& field_type = it->second;
        return field_type.GetName();
    }
    return std::string();
}

void kt::sd_definition_loader::process_namespace(service_definitions::xml_namespace namespace_) {
    if (namespace_.GetName().compare("orc")) {
        //todo
    }

    services_type_t& services = namespace_.GetServices_type();
    std::list<service_type_t>& service_list = services.get_service();
    for (service_type_t& service : service_list) {
        //todo
    }

    fields_type_t& fields_type = namespace_.GetFields_type();
    std::list<field_type_t>& field_list = fields_type.get_field();

    int namespace_value = namespace_.GetValue();
    std::string prefix = namespace_.GetName() + ".fields.";
    for (field_type_t& field : field_list) {
        std::string tag = field.GetTag().substr(prefix.size());
        std::string field_namespace = field.GetTag().substr(0, field.GetTag().find('.'));
        if (namespace_.GetName().compare(field_namespace) == 0) {
            int itag = atoi(tag.c_str());
            int tag_with_namespace = itag + namespace_value;
            _fields.insert(std::make_pair(tag_with_namespace, field));
            _field_tags.insert(std::make_pair(field.GetName(), tag_with_namespace));
        } else {
            int itag = atoi(tag.c_str());
            _fields.insert(std::make_pair(itag, field));
            _field_tags.insert(std::make_pair(field.GetName(), itag));
        }
    }

    size_t size = _fields.size();
    size_t size2 = _field_tags.size();
    //todo
    types_type_t& types_type = namespace_.GetTypes_type();
    std::list<type_t>& types = types_type.GetTypes();
    for(type_t& t : types) {
        _types.insert(std::pair<std::string,std::string>(t.GetName(),t.GetType()));
    }

}

