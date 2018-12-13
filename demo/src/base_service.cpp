/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_service.cpp
 * Author: kecy
 * 
 * Created on 2018年11月30日, 下午2:45
 */

#include "base_service.h"
#include "sd/data/sd_constants.h"
#include "log.h"
#include "orcfix_msg_dispatcher.h"
kt::base_service::base_service(int service_tag,orcfix_msg_dispatcher* call_back)
:_service_tag(service_tag)
,_call_back(call_back)
{
}

kt::base_service::base_service(const base_service& orig) {
}

kt::base_service::~base_service() {
}

void kt::base_service::on_message(kt::sd_message_t& sd_message_, kt::user& user_) {
    int message_type = sd_message_.GetMessage_tag();
    
    //TODO
    
    switch(message_type) {
        case sd_constants_t::MessageTypes::LoginReq:
            on_login_req(sd_message_, user_);
            break;
        case sd_constants_t::MessageTypes::LogoutReq:
            //on_logout_req(sd_message_);
            break;
        default:
            handle_message(sd_message_, user_);
            break;
    }
}

void kt::base_service::send(kt::sd_message_t& sd_message_, kt::user& user_) {
    bool ret = _call_back->send_sd_message(sd_message_,user_);
    if (!ret) {
        LOG_ERROR("Send message error");
    }
}

void kt::base_service::on_login_req(kt::sd_message_t& sd_message_,kt::user& user_) {
    //user user_(sd_message_.get_string(sd_constants_t::fields.Username), sd_message_.get_string(sd_constants_t::fields.Group));
    kt::sd_message_t reply_message(kt::sd_constants_t::MessageTypes::LoginRsp, this->_service_tag, sd_message_.GetOperation_id());
    field_value_t* value = sd_message_.get_value(sd_constants_t::fields.RequestId);
    if(value) {
        reply_message.add(sd_constants_t::fields.RequestId, *value);
    }
    reply_message.add(sd_constants_t::fields.ServiceVersion, "1.10");
    reply_message.add(sd_constants_t::fields.ServiceImplVersion, "1.0");
    reply_message.add(sd_constants_t::fields.MessageEncoding, "UTF-8");
    
    this->send(reply_message,user_);
    
    LOG_INFO("on_login_req {}", user_.to_string());
    auto it = _connected_users.find(user_.GetRouter_id());
    if(it != _connected_users.end()) {
        if(it->second.to_string() == user_.to_string()) {
            LOG_ERROR("User already logged: {}", user_.to_string());
            return;
        }
        it->second = user_;
    } else {
        _connected_users.insert(std::make_pair(user_.GetRouter_id(), user_));
    }

    
    this->handle_login(user_);
}

void kt::base_service::disconnect(kt::user user_) {
    auto it = _connected_users.find(user_.GetRouter_id());
    if(it != _connected_users.end() && it->second.to_string() == user_.to_string()) {
        handle_logout(user_);
        _connected_users.erase(it);
    }
}
