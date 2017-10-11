/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   update_thread.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月20日, 下午4:16
 */

#include "update_thread.h"

update_thread::update_thread(zmq::context_t* ctx)
: zmq_poller_reactor(ctx)
 {
}

update_thread::~update_thread() {

}

bool update_thread::init() {
    timers_add(timer_test, 1 * 1000, this);
    timers_add(timer_test2, 2 * 1000, this);
    timers_add(timer_test3, 10 * 1000, this);
    return true;
}

void update_thread::before_end() {
    LOG_INFO("update_thread end");
}

bool update_thread::before_start() {
    LOG_INFO("update_thread before_start");
    return true;
}

void update_thread::zmq_timer_event(int id_) {
    if(id_ == timer_test) {
        LOG_INFO("timer_test");
    }
    if(id_ == timer_test2) {
        LOG_INFO("timer_test2");
    }
    if(id_ == timer_test3) {
        LOG_INFO("timer_test3, stop thread");
        this->stop();
    }
}



