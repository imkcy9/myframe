/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_poller_reactor.cpp
 * Author: chengyeke
 * 
 * Created on 2017年8月31日, 下午2:10
 */

#include "zmq_poller_reactor.h"

zmq_poller_reactor::zmq_poller_reactor(zmq::context_t* ctx)
: m_ctx(ctx)
, m_stop(false) {
}

zmq_poller_reactor::~zmq_poller_reactor() {
}

void zmq_poller_reactor::run() {

    zmq::pollitem_t items[] = {
        {0, m_mailbox.get_fd(), ZMQ_POLLIN, 0}
    };
    bool stop = false;
    while (!stop) {
        zmq::message_t message;
        
        long wait_time = timers_timeout();
        if(wait_time == 0) {
            timers_execute();
            wait_time = timers_timeout();
        }
        int rc = zmq::poll(items, 1, wait_time);
        if (rc == 0) {
            LOG_INFO("timer out ");
            continue;
        }

        if (items [0].revents & ZMQ_POLLIN) {
            char c;
            int rc = m_mailbox.recv(&c, 0);
            while (rc == 0 || errno == EINTR) {
                if (rc == 0) {
                    LOG_INFO("recv {} ", c);
                    if(c == 'k')
                        stop = true;
                }
                rc = m_mailbox.recv(&c, 0);
            }
            assert(rc != 0 && errno == EAGAIN);
            
        }
    }
}

void zmq_poller_reactor::stop() {
    char c = 'k';
    m_mailbox.send(c);
}

void zmq_poller_reactor::timer_event(int id_) {
    LOG_WARN("timer id {}",id_);
}


