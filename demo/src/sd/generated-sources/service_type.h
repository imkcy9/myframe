/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   service_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午6:04
 */

#ifndef SERVICE_TYPE_H
#define SERVICE_TYPE_H

#include <string>

namespace kt {

    class service_type_t {
    public:

        std::string GetComment() const {
            return _comment;
        }

        void SetComment(std::string _comment) {
            this->_comment = _comment;
        }

        std::string GetExtends() const {
            return _extends;
        }

        void SetExtends(std::string _extends) {
            this->_extends = _extends;
        }

        std::string GetName() const {
            return _name;
        }

        void SetName(std::string _name) {
            this->_name = _name;
        }

        std::string GetTag() const {
            return _tag;
        }

        void SetTag(std::string _tag) {
            this->_tag = _tag;
        }

        std::string GetVersion() const {
            return _version;
        }

        void SetVersion(std::string _version) {
            this->_version = _version;
        }

    protected:
        std::string _version;
        std::string _tag;
        std::string _name;
        std::string _extends;
        std::string _comment;
    private:

    };
}


#endif /* SERVICE_TYPE_H */

