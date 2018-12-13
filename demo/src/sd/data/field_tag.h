/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field_tag.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午1:59
 */

#ifndef FIELD_TAG_H
#define FIELD_TAG_H
#include "field_enum_type.h"
#include <string>
#include "../sd_definition_loader.h"
#include <vector>
namespace kt {
    const int CACHE_SIZE = 16000;
    class field_tag_t {
    public:
        field_tag_t() {
            
        }
        field_tag_t(std::string name, int tag, field_enum_type_t field_type) {
            this->_name = name;
            this->_tag = tag;
            this->_field_type = field_type;
        }
        
        ~field_tag_t() {}
        
        field_enum_type_t GetField_type() const {
            return _field_type;
        }

        std::string GetName() const {
            return _name;
        }

        int GetTag() const {
            return _tag;
        }

        static field_tag_t value_of(int tag) {
            field_tag_t* field_tag = NULL;
            if (tag < CACHE_SIZE && tag >= 0) {
                field_tag = field_vec_cache[tag];
                if (field_tag != NULL) {
                    return *field_tag;
                }
            }

            
            
            kt::field_enum_type_t field_type = sd_definition_loader::get_field_type(tag);
            std::string name = sd_definition_loader::get_field_name(tag);
            field_tag = new field_tag_t(name, tag, field_type);
            //todo;
            if(tag < CACHE_SIZE) {
                field_vec_cache[tag] = field_tag;
            }
            return *field_tag;
        }
        
        static field_tag_t value_of(std::string tag) {
            return value_of(sd_definition_loader::get_field_tag(tag));
        }

    private:




        //static field_tag_t** field_cache;
        //const int CACHE_SIZE = 16000;
        static std::vector<field_tag_t*> field_vec_cache;
        field_enum_type_t _field_type;
        std::string _name;
        int _tag;
    };

}


#endif /* FIELD_TAG_H */

