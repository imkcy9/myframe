/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_monitor.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月27日, 下午5:52
 */

#include "zmq_monitor.h"
#include "log.h"


zmq_monitor::zmq_monitor() {
}

zmq_monitor::~zmq_monitor() {
}

void zmq_monitor::on_event_listening(const zmq_event_t& event_, const char* addr_) {
    LOG_DEBUG("Server listening on addr: {}",addr_);
}

void zmq_monitor::on_event_accepted(const zmq_event_t& event_, const char* addr_) {
    LOG_DEBUG("Server accepted : {}",addr_);
}
