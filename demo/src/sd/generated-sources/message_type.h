/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   message_type.h
 * Author: kecy
 *
 * Created on 2018年11月15日, 下午6:01
 */

#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <string>
namespace kt {

    class message_type_t {
    public:

        std::string GetComment() const {
            return _comment;
        }

        void SetComment(std::string _comment) {
            this->_comment = _comment;
        }

        std::string GetDescription() const {
            return _description;
        }

        void SetDescription(std::string _description) {
            this->_description = _description;
        }

        std::string GetTag() const {
            return _tag;
        }

        void SetTag(std::string _tag) {
            this->_tag = _tag;
        }

    protected:
        std::string _description;
        std::string _tag;
        std::string _comment;

    private:

    };
}


#endif /* MESSAGE_TYPE_H */

