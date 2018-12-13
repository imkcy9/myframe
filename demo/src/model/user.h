/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   user.h
 * Author: kecy
 *
 * Created on 2018年11月29日, 下午10:41
 */

#ifndef USER_H
#define USER_H
#include <string>
#include "blob.h"
namespace kt {
    class user {
    public:
        user(){}
        user(std::string user_name, std::string group, kt::blob_t router_id)
        :_user_name(user_name)
        ,_group(group)
        ,_router_id(router_id)
        {}
        ~user(){}
        
        std::string GetGroup() const {
            return _group;
        }

        std::string GetUser_name() const {
            return _user_name;
        }

        kt::blob_t GetRouter_id() const {
            return _router_id;
        }

        std::string to_string() const {
            return _user_name + ":" + _group;
        }
        
        bool operator<(const user& user_)  {
            const int cmpres = to_string().compare(user_.to_string());
            if(cmpres == 0) {
                return _router_id < user_.GetRouter_id();
            }
            return cmpres < 0;
        }
    private:
        std::string _user_name;
        std::string _group;
        kt::blob_t _router_id;
    };
}


#endif /* USER_H */

