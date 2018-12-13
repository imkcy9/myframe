/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   service_definitions.cpp
 * Author: kecy
 * 
 * Created on 2018年11月15日, 下午5:22
 */

#include "service_definitions.h"
#include <iostream>
#include <string.h>
#include <assert.h>

using namespace tinyxml2;

kt::service_definitions::service_definitions() {
}

kt::service_definitions::service_definitions(const service_definitions& orig) {
}

kt::service_definitions::~service_definitions() {
}

std::list<kt::service_definitions::xml_namespace> kt::service_definitions::getNamespaces() {
    return _namespaces;
}

bool kt::service_definitions::load_resource(std::string _xml_file) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(_xml_file.c_str());
    tinyxml2::XMLError result = doc.ErrorID();
    if (result != XML_SUCCESS)
        return false;

    XMLNode* root = doc.FirstChildElement("service-definitions");
    if (root) {
        this->init(root->FirstChild());
    }


//    for (XMLNode* ele = doc.FirstChildElement("service-definitions")->FirstChild(); ele; ele = ele->NextSibling()) {
//        std::cout << ele->Value() << std::endl;
//        char* str;
//        const XMLAttribute* a = ele->ToElement()->FirstAttribute();
//        while (a) {
//
//            std::cout << a->Name() << std::endl;
//            std::cout << a->Value() << std::endl;
//            a = a->Next();
//        }
//
//        if (strcmp("namespace", ele->Value()) == 0) {
//            XMLNode* field = ele->FirstChild();
//            std::cout << field->Value() << std::endl;
//            field = field->FirstChild();
//            while (field) {
//                const XMLAttribute* f = field->ToElement()->FirstAttribute();
//                while (f) {
//                    std::cout << f->Name() << ":" << f->Value() << std::endl;
//                    f = f->Next();
//                }
//                field = field->NextSibling();
//            }
//        }
//    }
    return true;
}

void kt::service_definitions::init(XMLNode* node) {
    if (!node)
        return;

    for (; node; node = node->NextSibling()) {
        if (strcmp("namespace", node->Value()) == 0) {
            this->add_namespace(node);
        }
        if (strcmp("import", node->Value()) == 0) {

        }
    }
}

void kt::service_definitions::add_namespace(tinyxml2::XMLNode* node) {
    assert(strcmp("namespace", node->Value()) == 0);

    xml_namespace namespace_t;
    namespace_t.SetName(node->ToElement()->FindAttribute("name")->Value());
    namespace_t.SetValue(node->ToElement()->FindAttribute("value")->IntValue());
    for (XMLNode* type = node->FirstChild(); type; type = type->NextSibling()) {
        if (strcmp("fields", type->Value()) == 0) {
            this->add_fields_to_namespace(namespace_t, type);
        }
        if (strcmp("types", type->Value()) == 0) {
            this->add_types_to_namespace(namespace_t, type);
        }
        if (strcmp("messages", type->Value()) == 0) {
            this->add_messages_to_namespace(namespace_t, type);
        }
        if (strcmp("services", type->Value()) == 0) {
            this->add_services_to_namespace(namespace_t, type);
        }
    }
    _namespaces.push_back(namespace_t);
}

void kt::service_definitions::add_fields_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node) {
    fields_type_t& fields_type = des_namespace.GetFields_type();
    std::list<field_type_t>& fiels_list = fields_type.get_field();
    for(XMLElement* field = node->FirstChildElement("field"); field != NULL ; field = field->NextSiblingElement("field")) {
        assert(strcmp("field",field->Name()) == 0);
        field_type_t field_type;
        
        //std::cout << field->Value() << std::endl;
        //std::cout << field->ToElement()->FindAttribute("name")->Value() << std::endl;
        
        const XMLAttribute* attribute = field->ToElement()->FindAttribute("name");
        if(attribute)  {
            field_type.SetName(attribute->Value());
        }
        
        attribute = field->ToElement()->FindAttribute("tag");
        if(attribute)  {
            field_type.SetTag(attribute->Value());
        }
        
        attribute = field->ToElement()->FindAttribute("type");
        if(attribute)  {
            field_type.SetType(attribute->Value());
        }
        
        attribute = field->ToElement()->FindAttribute("comment");
        if(attribute)  {
            field_type.SetComment(attribute->Value());
        }
        
        attribute = field->ToElement()->FindAttribute("size");
        if(attribute)  {
            field_type.SetSize(attribute->Value());
        }
        
        attribute = field->ToElement()->FindAttribute("array");
        if(attribute)  {
            if(strcmp(attribute->Value(), "true") == 0) {
                field_type.SetArray(true);
            }
        }
        
        fiels_list.push_back(field_type);
    }
}

void kt::service_definitions::add_types_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node) {
    types_type_t& types_type = des_namespace.GetTypes_type();
    std::list<type_t>& types = types_type.GetTypes();
    for(XMLElement* field = node->FirstChildElement("enums"); field != NULL ; field = field->NextSiblingElement("enums")) {
        std::string field_name = field->Name();
        for(XMLElement* ele = field->FirstChildElement("enum"); ele != NULL ; ele = ele->NextSiblingElement("enum")) {
            std::string field = ele->Name();
            std::string name = ele->FindAttribute("name")->Value();
            std::string type = ele->FindAttribute("type")->Value();
            type_t t;
            t.SetName(name);
            t.SetType(type);
            types.push_back(t);
        }
    }
}

void kt::service_definitions::add_messages_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node) {

}

void kt::service_definitions::add_services_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node) {

}