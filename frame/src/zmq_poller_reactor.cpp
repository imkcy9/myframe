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
:m_ctx(ctx)
,m_signal(*ctx,ZMQ_SUB)
,m_stop(false){
}


zmq_poller_reactor::~zmq_poller_reactor() {
}

bool zmq_poller_reactor::init_before_start() {
    m_signal.setsockopt(ZMQ_SUBSCRIBE,"",0);
    m_signal.connect("inproc://signal");
    return true;
}

void zmq_poller_reactor::run() {
    
    
    std::function<void(void)> handler1 = std::bind(&zmq_poller_reactor::event_handler, this);
    poller.add(m_signal,ZMQ_POLLIN,handler1);
    while(!m_stop) {
        bool ret = poller.wait(std::chrono::milliseconds(1000));
        if(!ret) {
            LOG_INFO("poller timer out");
        }
    }
}

void zmq_poller_reactor::release_before_end() {

}

void zmq_poller_reactor::event_handler() {
    zmq::message_t msg;
    m_signal.recv(&msg);
    LOG_INFO("signal ddddddddddddddddddddddddd{}", (char*)msg.data());
}

bool zmq_poller_reactor::add_socket(zmq::socket_t& socket, std::function<void()()>& handler) {
    
}


