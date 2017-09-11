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

void timer_fun(int id_, void *arg) {
    timer* t = (timer*)arg;
    t->on_timer_event(id_,arg);
}

void timer::on_timer_event(int id_, void* arg) {
    timer_event(m_timerMap[id_]);
}

timer::timer() {
    timer_ = zmq_timers_new();
}

timer::~timer() {
    zmq_timers_destroy(&timer_);
}

int timer::timers_add(int id_, size_t interval) {
    int id = zmq_timers_add(timer_,interval,timer_fun,this);
    m_timerMap.insert(std::make_pair(id,id_));
    //vec_fun.push_back(handler);
    //assert(vec_fun.size() == id);
    return id;
}

long timer::timers_timeout() {
    return zmq_timers_timeout(timer_);
}


void timer::timers_execute() {
    zmq_timers_execute(timer_);
}



