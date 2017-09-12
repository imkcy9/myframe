/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_poller_reactor.h
 * Author: chengyeke
 *
 * Created on 2017年8月31日, 下午2:10
 */

#ifndef ZMQ_POLLER_REACTOR_H
#define ZMQ_POLLER_REACTOR_H
#include "thread.h"
#include <zmq.hpp>
#include "log.h"
#include "timer/mailbox.h"
#include "timer.h"

class zmq_poller_reactor : public thread , public timer {
public:
    zmq_poller_reactor(zmq::context_t* ctx);

    virtual ~zmq_poller_reactor();

    virtual void run();

    virtual void timer_event(int id_);
    
    void stop();
    
    mailbox_t<char> m_mailbox;
private:
    zmq::context_t* m_ctx;
    bool m_stop;
};

#endif /* ZMQ_POLLER_REACTOR_H */

