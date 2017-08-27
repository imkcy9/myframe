/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timer.cpp
 * Author: kcy
 * 
 * Created on 2017年8月24日, 下午11:22
 */

#include "timer.h"
#include <zmq.h>
#include <assert.h>
timer::timer() {
    timer_ = zmq_timers_new();
}

timer::~timer() {
    zmq_timers_destroy(&timer_);
}

int timer::timers_add(size_t interval, timer_fn* handler, void* arg) {
    int id = zmq_timers_add(timer_,interval,(zmq_timer_fn*)(handler),arg);
    //vec_fun.push_back(handler);
    //assert(vec_fun.size() == id);
    return id;
}

void timer::timers_execute() {
    zmq_timers_execute(timer_);
}



