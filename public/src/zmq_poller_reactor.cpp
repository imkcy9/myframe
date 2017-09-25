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

#include <zmq.h>

#include "zmq_poller_reactor.h"

zmq_poller_reactor::zmq_poller_reactor(zmq::context_t* ctx)
: mailbox_event(&m_mailbox)
,m_ctx(ctx)
, m_stop(false) {
}

zmq_poller_reactor::~zmq_poller_reactor() {
}

void zmq_poller_reactor::run() {

    int socket_count = m_poll_sockets.size() + 1;
    zmq::pollitem_t *items = (zmq::pollitem_t *)malloc(sizeof(zmq::pollitem_t) * (m_poll_sockets.size() + 1));
    items[0] = {0, m_mailbox.get_fd(), ZMQ_POLLIN, 0};
    for(size_t i = 0; i < m_poll_sockets.size(); i++) {
        items[i+1] = {*(m_poll_sockets[i].first), 0, ZMQ_POLLIN, 0};
    }

    while (!m_stop) {
        zmq::message_t message;
        
        long wait_time = timers_timeout();
        if(wait_time == 0) {
            timers_execute();
            wait_time = timers_timeout();
        }
        int rc = zmq::poll(items, socket_count, wait_time);
        if (rc == 0) {
            //LOG_INFO("timer out ");
            continue;
        }

        if (items [0].revents & ZMQ_POLLIN) {
            event event_;
            int rc = m_mailbox.recv(&event_, 0);
            while (rc == 0 || errno == EINTR) {
                if (rc == 0) {
                    LOG_DEBUG("mailbox event recv {} ", event_.type);
                    process_event(event_);
                }
                rc = m_mailbox.recv(&event_, 0);
            }
            assert(rc != 0 && errno == EAGAIN);
        }
        
        for(size_t i = 0; i < m_poll_sockets.size(); i++) {
            
            if(items [i+1].revents & ZMQ_POLLIN) {
                m_poll_sockets[i].second->zmq_in_event(m_poll_sockets[i].first);
            }
        }
    }
    
    free(items);
}

void zmq_poller_reactor::stop() {
    send_term();
}

void zmq_poller_reactor::add_socket(zmq::socket_t* socket, zmq_poll_events* event) {
    //assert(socket);
    //m_poll_sockets.push_back(socket);
    m_poll_sockets.push_back(std::make_pair(socket,event));
}

mailbox_t<event>* zmq_poller_reactor::get_mailbox() {
    return &m_mailbox;
}

void zmq_poller_reactor::process_term() {
    m_stop = true;
}

