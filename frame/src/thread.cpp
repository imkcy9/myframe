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

thread::thread()
: thread_handler(NULL)
,is_start(false) {
}

thread::~thread() {
}

void thread::start() {
    if(is_running())
        return;
    
    is_start = true;
    thread_handler = zmq_threadstart(worke_routine, this);
}

void thread::join() {
    zmq_threadclose(thread_handler);
}

bool thread::init_before_start() {
    return true;
}

void thread::release_before_end() {

}


void thread::worke_routine(void* arg_) {
    bool ret = ((thread*) arg_)->init_before_start();
    if (ret) {
        ((thread*) arg_)->run();
    }
    ((thread*) arg_)->release_before_end();
    ((thread*) arg_)->is_start = false;
}

