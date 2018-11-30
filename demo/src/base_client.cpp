/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_client.cpp
 * Author: kecy
 * 
 * Created on 2018年11月30日, 下午2:12
 */

#include "base_client.h"
#include "sd/data/sd_constants.h"

kt::base_client::base_client() {
}

kt::base_client::base_client(const base_client& orig) {
}

kt::base_client::~base_client() {
}

kt::sd_message_t kt::base_client::get_login(kt::user user_) {
    kt::sd_message_t login(kt::sd_constants_t::MessageTypes::LoginReq,
            this->get_service(),
            kt::sd_constants_t::Operations::LoginOperation);
    login.add(sd_constants_t::fields.Username, user_.GetUser_name());
    login.add(sd_constants_t::fields.Group, user_.GetGroup());
    login.add(sd_constants_t::fields.ServiceVersion, "2.0");
    login.add(sd_constants_t::fields.ServiceImplVersion, "2.0");
    login.add(sd_constants_t::fields.ApplicationVersion, "2018.11.30");
    
    return login;
}

void kt::base_client::logout() {
    kt::sd_message_t logout(kt::sd_constants_t::MessageTypes::LogoutReq,
            this->get_service(),
            kt::sd_constants_t::Operations::LogoutOperation);
    
    this->send(logout);
}

void kt::base_client::send(kt::sd_message_t& sd_message_) {

}

void kt::base_client::on_message(kt::sd_message_t& sd_message_) {
    
}
