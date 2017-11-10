/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   i_poll_event.h
 * Author: chengyeke
 *
 * Created on 2017年9月19日, 上午11:22
 */

#ifndef I_POLL_EVENT_H
#define I_POLL_EVENT_H
#include "zmq.hpp"
class zmq_poll_events {
public:
    zmq_poll_events(){};
    virtual ~zmq_poll_events(){};
    
    virtual void zmq_in_event(zmq::socket_t* socket) {assert(false);};
    virtual void zmq_out_event() {assert(false);};
    virtual void zmq_timer_event(int id_) {assert(false);};
private:

};

#endif /* I_POLL_EVENT_H */

