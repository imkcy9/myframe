/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   types_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午5:57
 */

#ifndef TYPES_TYPE_H
#define TYPES_TYPE_H
#include "type.h"

namespace kt {
    class types_type_t {
    public:
        std::list<type_t>& GetTypes() {
            return _types;
        }

    protected:
        std::list<type_t> _types;
    };
}


#endif /* TYPES_TYPE_H */

