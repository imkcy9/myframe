/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   services_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午6:04
 */

#ifndef SERVICES_TYPE_H
#define SERVICES_TYPE_H

#include <list>
#include "service_type.h"
namespace kt {
    class services_type_t {
    public:
        std::list<service_type_t>& get_service() {
            return this->_service;
        }
    protected:
        std::list<service_type_t> _service;
    };
}


#endif /* SERVICES_TYPE_H */

