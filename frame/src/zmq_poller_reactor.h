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
class zmq_poller_reactor : public thread {
public:
    zmq_poller_reactor(zmq::context_t* ctx);

    virtual ~zmq_poller_reactor();
    

    virtual bool init_before_start();

    virtual void run();

    virtual void release_before_end();
    
    bool add_socket(zmq::socket_t &socket, std::function<void(void)> &handler);

    void event_handler();

private:
    zmq::context_t* m_ctx;
    zmq::socket_t m_signal;
    bool m_stop;
    zmq::poller_t poller;
};

#endif /* ZMQ_POLLER_REACTOR_H */

