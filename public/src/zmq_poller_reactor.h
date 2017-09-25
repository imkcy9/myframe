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
#include "zmq_poll_events.h"
#include "mailbox_event.h"

class zmq_poller_reactor : public thread_t , public timer, public mailbox_event, public zmq_poll_events {
public:
    zmq_poller_reactor(zmq::context_t* ctx);

    virtual ~zmq_poller_reactor();

    virtual void run();
    
    virtual void stop();
    
    void add_socket(zmq::socket_t* socket, zmq_poll_events* event);
    
    mailbox_t<event>* get_mailbox();
    
private:
    
    void process_term() override;

    mailbox_t<event> m_mailbox;
    zmq::context_t* m_ctx;
    bool m_stop;
    //std::vector<zmq::socket_t*> m_poll_sockets;
    std::vector<std::pair<zmq::socket_t*,zmq_poll_events*>> m_poll_sockets;
};

#endif /* ZMQ_POLLER_REACTOR_H */
