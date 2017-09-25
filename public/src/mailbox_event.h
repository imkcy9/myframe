/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mailbox_event.h
 * Author: chengyeke
 *
 * Created on 2017年9月20日, 下午4:49
 */

#ifndef MAILBOX_EVENT_H
#define MAILBOX_EVENT_H
#include "timer/mailbox.h"

struct event {
    enum type_{
        term = 0,
        update_innercode
    } type;
    void* metadata;
};

class mailbox_event {
public:
    mailbox_event(mailbox_t<event>* mailbox);
    virtual ~mailbox_event();
    
    void send_term();
    void send_update_innercode(void* metadata);

    void process_event(event& event_);
private:
    
    virtual void process_term(){};
    virtual void process_update_innercode(void* metadata){};
    mailbox_t<event>* m_mailbox;
};

#endif /* MAILBOX_EVENT_H */

