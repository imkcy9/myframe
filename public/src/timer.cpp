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
#include <vector>
#include "zmq_poll_events.h"

void timer_fun(int id_, void *arg) {
    timer* t = (timer*)arg;
    t->on_timer_event(id_,arg);
}

void timer::on_timer_event(int id_, void* arg) {
    auto it = m_timerMap.find(id_);
    if( it != m_timerMap.end()) {
        it->second.event->zmq_timer_event(it->second.id_);
    }
}

timer::timer() {
    timer_ = zmq_timers_new();
}

timer::~timer() {
    if(timer_) {
        zmq_timers_destroy(&timer_);
    }
}

int timer::timers_add(int id_, size_t interval, zmq_poll_events* event) {
    int id = zmq_timers_add(timer_,interval,timer_fun,this);
    timer_info timer_info_ = {id_, timer_fun, event};
    m_timerMap.insert(std::pair<int,timer_info>(id,timer_info_));
    //vec_fun.push_back(handler);
    //assert(m_timerMap.size() == id);
    return id;
}

int timer::timers_cancel(int id_,zmq_poll_events* event) {
    for (auto it = m_timerMap.begin(); it != m_timerMap.end(); it++) {
        if(it->second.id_ == id_ && it->second.event == event) {
            zmq_timers_cancel(timer_,it->first);
            m_timerMap.erase(it);
            return 0;
        }
    }
    return -1;
}


long timer::timers_timeout() {
    return zmq_timers_timeout(timer_);
}


void timer::timers_execute() {
    zmq_timers_execute(timer_);
}



