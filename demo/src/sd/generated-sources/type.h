/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   type.h
 * Author: kecy
 *
 * Created on 2018年12月7日, 下午1:14
 */

#ifndef TYPE_H
#define TYPE_H

namespace kt{
    class type_t {
    public:
        type_t(){}
        virtual ~type_t(){}
        std::string GetName() const {
            return _name;
        }

        void SetName(std::string _name) {
            this->_name = _name;
        }

        std::string GetType() const {
            return _type;
        }

        void SetType(std::string _type) {
            this->_type = _type;
        }

    private:
        std::string _name;
        std::string _type;
    };
}


#endif /* TYPE_H */

