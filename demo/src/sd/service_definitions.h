/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   service_definitions.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午5:22
 */

#ifndef SERVICE_DEFINITIONS_H
#define SERVICE_DEFINITIONS_H
#include <string>
#include <list>
#include "generated-sources/fields_type.h"
#include "generated-sources/messages_type.h"
#include "generated-sources/services_type.h"
#include "generated-sources/types_type.h"
#include "../common/tinyxml2.h"
namespace kt {

    class service_definitions {
    public:
        service_definitions();
        service_definitions(const service_definitions& orig);
        virtual ~service_definitions();

        bool load_resource(std::string _xml_file);

        class xml_namespace {
        public:

            xml_namespace() {
            }

            ~xml_namespace() {
            };

            std::string GetComment()  {
                return _comment;
            }

            void SetComment(std::string _comment) {
                this->_comment = _comment;
            }

            std::string GetDescription()  {
                return _description;
            }

            void SetDescription(std::string _description) {
                this->_description = _description;
            }

            kt::fields_type_t& GetFields_type() {
                return _fields_type;
            }

            void SetFields_type(kt::fields_type_t _fields_type) {
                this->_fields_type = _fields_type;
            }

            kt::messages_type_t& GetMessages_type()  {
                return _messages_type;
            }

            void SetMessages_type(kt::messages_type_t _messages_type) {
                this->_messages_type = _messages_type;
            }

            std::string GetName() const {
                return _name;
            }

            void SetName(std::string _name) {
                this->_name = _name;
            }

            kt::services_type_t& GetServices_type()  {
                return _services_type;
            }

            void SetServices_type(kt::services_type_t _services_type) {
                this->_services_type = _services_type;
            }

            kt::types_type_t& GetTypes_type()  {
                return _types_type;
            }

            void SetTypes_type(kt::types_type_t _types_type) {
                this->_types_type = _types_type;
            }

            int GetValue() const {
                return _value;
            }

            void SetValue(int _value) {
                this->_value = _value;
            }


        protected:
            std::string _description;
            kt::fields_type_t _fields_type;
            kt::types_type_t _types_type;
            kt::messages_type_t _messages_type;
            kt::services_type_t _services_type;
            std::string _name;
            int _value;
            std::string _comment;
        private:
        };
        std::list<kt::service_definitions::xml_namespace> getNamespaces();
    protected:
        std::list<kt::service_definitions::xml_namespace> _namespaces;

    private:
        void init(tinyxml2::XMLNode* node);
        void add_namespace(tinyxml2::XMLNode* node);
        void add_fields_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node);
        void add_types_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node);
        void add_messages_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node);
        void add_services_to_namespace(xml_namespace& des_namespace, tinyxml2::XMLNode* node);
    };


}

#endif /* SERVICE_DEFINITIONS_H */

