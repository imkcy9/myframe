/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_client.h
 * Author: kecy
 *
 * Created on 2018年11月30日, 下午2:12
 */

#ifndef BASE_CLIENT_H
#define BASE_CLIENT_H

#include "model/user.h"
#include "sd/data/sd_message.h"


namespace kt {
class base_client {
public:
    base_client();
    base_client(const base_client& orig);
    virtual ~base_client();
    virtual int get_service() = 0;
    
    void send(kt::sd_message_t& sd_message_);
    
    void on_message(kt::sd_message_t& sd_message_);
protected:
    kt::sd_message_t get_login(kt::user user_);
private:
    void logout();
};
}
#endif /* BASE_CLIENT_H */

