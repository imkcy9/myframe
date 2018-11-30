/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fields_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午5:51
 */

#ifndef FIELDS_TYPE_H
#define FIELDS_TYPE_H

#include <list>
#include "field_type.h"
namespace kt {

    class fields_type_t {
    public:

        std::list<field_type_t>& get_field() {
            return _field;
        }
    protected:
        std::list<field_type_t> _field;
    private:

    };
}


#endif /* FIELDS_TYPE_H */

