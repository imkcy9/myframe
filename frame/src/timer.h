/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timer.h
 * Author: kcy
 *
 * Created on 2017年8月24日, 下午11:22
 */

#ifndef TIMER_H
#define TIMER_H
#include <cstddef>
#include <map>

typedef void (timer_fn)(int timer_id, void *arg);
class timer {
public:
    timer();
    virtual ~timer();
    int timers_add(int id_, size_t interval);
    long timers_timeout();
    void timers_execute();
    
    void on_timer_event(int id_, void *arg);
    virtual void timer_event(int id_) = 0;
private:
    void* timer_;
    struct timer_info {
        int id_;
        timer_fn* fun_;
    };
    std::map<int,int> m_timerMap;
    //void timer_fun(int timer_id, void *arg);
    
    //std::vector<timer_fn*> vec_fun;
};

#endif /* TIMER_H */

