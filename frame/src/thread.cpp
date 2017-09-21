/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   thread.cpp
 * Author: kcy
 * 
 * Created on 2017年8月24日, 下午10:47
 */

#include "thread.h"
#include <zmq.h>

thread_t::thread_t()
: thread_handler(NULL)
,is_start(false) {
}

thread_t::~thread_t() {
}

bool thread_t::init() {
    return true;
}

void thread_t::start() {
    if(is_running())
        return;
    
    is_start = true;
    thread_handler = zmq_threadstart(worke_routine, this);
}

void thread_t::join() {
    if(thread_handler)
        zmq_threadclose(thread_handler);
}

bool thread_t::before_start() {
    return true;
}

void thread_t::before_end() {

}


void thread_t::worke_routine(void* arg_) {
    bool ret = ((thread_t*) arg_)->before_start();
    if (ret) {
        ((thread_t*) arg_)->run();
    }
    ((thread_t*) arg_)->before_end();
    ((thread_t*) arg_)->is_start = false;
}

