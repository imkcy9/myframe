/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field_struct.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午2:04
 */

#ifndef FIELD_STRUCT_H
#define FIELD_STRUCT_H
#include <unordered_map>
#include <list>
#include <float.h>
#include "field_value.h"
#include "field.h"
#include "field_enum_type.h"
namespace kt {
    class field_struct_t {
    public:
        field_struct_t(bool inbound) {
            this->_inbound = inbound;
            //_fields.resize(64);
        }
        field_struct_t() {
            this->_inbound = true;
        }
        
        ~field_struct_t() {
            
        }
        
        bool add(field_tag_t tag, field_value_t value) {
            field_enum_type_t type = tag.GetField_type();
            if(type != value.GetType()) {
                if(!(type == field_enum_type_t::DATETIME && value.GetType() == field_enum_type_t::UINT64) && 
                        !(type == field_enum_type_t::DATE && value.GetType() == field_enum_type_t::UINT32) &&
                        !(type == field_enum_type_t::CHAR && value.GetType() == field_enum_type_t::INT8)) {
                    return false;
                }
            }
            _fields.push_back(field_t(tag,value));
            if(_inbound) {
                _field_map.insert(std::make_pair(tag.GetTag(), value));
            }
            
            return true;
        }
        
        bool add(field_tag_t tag, std::string value) {
            return this->add(tag, field_value_t::value_of(value));
        }
        
        bool add(field_tag_t tag, bool value) {
            return this->add(tag, field_value_t::value_of(value, field_enum_type_t::BOOLEAN));
        }
        
        bool add(field_tag_t tag, double value) {
            if(tag.GetField_type() == field_enum_type_t::REAL32) {
                return this->add(tag, field_value_t::value_of(value, field_enum_type_t::REAL32));
            } else {
                return this->add(tag, field_value_t::value_of(value, field_enum_type_t::REAL64));
            }
        }
        
        bool add(field_tag_t tag, long value) {
            return this->add(tag, field_value_t::value_of(value, field_enum_type_t::INT64));
        }
        
        bool add(field_tag_t tag, field_struct_t value) {
            return this->add(tag, field_value_t::value_of(&value));
        }
        
        field_value_t* get_value(field_tag_t& tag) {
            if(_inbound) {
                auto it = _field_map.find(tag.GetTag());
                if(it != _field_map.end())
                    return &it->second;
            }
            
            for(field_t& field : _fields) {
                if(field.GetTag().GetTag() == tag.GetTag()) {
                    return &field.GetField_value();
                }
            }
            return NULL;
        }
        
        std::string get_string(field_tag_t& tag) {
            field_value_t* value = this->get_value(tag);
            if(NULL == value)
                return std::string();
            
            return value->GetString_value();
        }
        
        double get_double(field_tag_t& tag) {
            field_value_t* value = this->get_value(tag);
            if(NULL == value)
                return DBL_MAX;
            
            return value->GetReal64_value();
        }
        
        std::list<field_t>& get_fields() {
            return _fields;
        }
        
        
    public:
        field_struct_t(std::list<field_t>& fields)
        :field_struct_t(true)
        {
            
            for(field_t& field : fields) {
                this->_fields.push_back(field);
            }
            for(field_t& field : fields) {
                _field_map.insert(std::make_pair(field.GetTag().GetTag(), field.GetField_value()));
            }
            
        }
    private:
        std::unordered_map<int, field_value_t> _field_map;
        std::list<field_t> _fields;
        bool _inbound;
    };
}


#endif /* FIELD_STRUCT_H */

