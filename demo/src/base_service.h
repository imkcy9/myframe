/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_service.h
 * Author: kecy
 *
 * Created on 2018年11月30日, 下午2:45
 */

#ifndef BASE_SERVICE_H
#define BASE_SERVICE_H
#include "model/user.h"
#include "sd/data/sd_message.h"
#include "orcfix_msg_dispatcher.h"
#include <map>

namespace kt {
class base_service {
public:
    base_service(int service_tag, orcfix_msg_dispatcher* call_back);
    base_service(const base_service& orig);
    virtual ~base_service();
    
    void on_message(kt::sd_message_t& sd_message_, kt::user& user_);
    
    virtual void handle_message(kt::sd_message_t& sd_message_, kt::user& user_) = 0;
    virtual void handle_logout(kt::user user_) = 0;
    virtual void handle_login(kt::user user_) = 0;
    void disconnect(kt::user user_);
    
    inline int get_service_tag(){return this->_service_tag;}
protected:
    void send(kt::sd_message_t& sd_message_, kt::user& user_);
    void on_login_req(kt::sd_message_t& sd_message_,kt::user& user_);
    std::map<kt::blob_t, kt::user> _connected_users;
private:
    int _service_tag;
    orcfix_msg_dispatcher* _call_back;
};
}
#endif /* BASE_SERVICE_H */

