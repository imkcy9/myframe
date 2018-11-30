/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午5:30
 */

#ifndef FIELD_TYPE_H
#define FIELD_TYPE_H
#include <string>
namespace kt {
    class field_type_t {
    public:
        std::string GetArray() const {
            return _array;
        }

        void SetArray(std::string _array) {
            this->_array = _array;
        }

        std::string GetComment() const {
            return _comment;
        }

        void SetComment(std::string _comment) {
            this->_comment = _comment;
        }

        std::string GetName() const {
            return _name;
        }

        void SetName(std::string _name) {
            this->_name = _name;
        }

        std::string GetSize() const {
            return _size;
        }

        void SetSize(std::string _size) {
            this->_size = _size;
        }

        std::string GetTag() const {
            return _tag;
        }

        void SetTag(std::string _tag) {
            this->_tag = _tag;
        }

        std::string GetType() const {
            return _type;
        }

        void SetType(std::string _type) {
            this->_type = _type;
        }

    protected:
        std::string _name;
        std::string _tag;
        std::string _type;
        std::string _size;
        std::string _comment;
        std::string _array;
    };
}



#endif /* FIELD_TYPE_H */

