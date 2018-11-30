/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午1:58
 */

#ifndef FIELD_H
#define FIELD_H
#include "field_value.h"
#include "field_tag.h"

namespace kt {

    class field_t {
    public:

        field_t() {

        }

        field_t(kt::field_tag_t tag, kt::field_value_t field_value) {
            this->_tag = tag;
            this->_field_value = field_value;
        }

        ~field_t() {

        }

        field_value_t& GetField_value() {
            return _field_value;
        }

        field_tag_t GetTag() const {
            return _tag;
        }

    private:
        field_tag_t _tag;
        field_value_t _field_value;
    };
}



#endif /* FIELD_H */

