/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mailbox_event.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月20日, 下午4:49
 */

#include "mailbox_event.h"

mailbox_event::mailbox_event(mailbox_t<event>* mailbox)
:m_mailbox(mailbox){
}

mailbox_event::~mailbox_event() {
}

void mailbox_event::send_term() {
    event event_;
    event_.type = event::term;
    m_mailbox->send(event_);
}

void mailbox_event::send_update_innercode(void* metadata) {
    event event_;
    event_.type = event::update_innercode;
    event_.metadata = metadata;
    m_mailbox->send(event_);
}

void mailbox_event::send_tradingday_changed(const char* tradingday) {
    event event_;
    event_.type = event::tradingday_changed;
    event_.metadata = (void*)tradingday;
    m_mailbox->send(event_);
}


void mailbox_event::process_event(event& event_) {
    switch(event_.type) {
        case event::term:
            process_term();
            break;
        case event::update_innercode:
            process_update_innercode(event_.metadata);
            break;
        case event::tradingday_changed:
            process_tradingday_changed((const char*)event_.metadata);
            break;
        default:
            break;
    }
}


