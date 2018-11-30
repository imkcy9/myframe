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
#include <map>

namespace kt {
class base_service {
public:
    base_service(int service_tag);
    base_service(const base_service& orig);
    virtual ~base_service();
    
    void on_message(kt::sd_message_t& sd_message_, kt::user& user_);
    
    virtual void handle_message(kt::sd_message_t& sd_message_) = 0;
    virtual void handle_logout(kt::user user_) = 0;
    virtual void handle_login(kt::user user_) = 0;
protected:
    void send(kt::sd_message_t& sd_message_, kt::blob_t& router_id_);
    void on_login_req(kt::sd_message_t& sd_message_,kt::user& user_);
private:
    std::map<kt::blob_t, kt::user> _connected_users;
    int _service_tag;
};
}
#endif /* BASE_SERVICE_H */

