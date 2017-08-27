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
#include <vector>

typedef void (timer_fn)(int timer_id, void *arg);
class timer {
public:
    timer();
    virtual ~timer();
    int timers_add(size_t interval, timer_fn* handler, void *arg);
    void timers_execute();
private:
    void* timer_;
    //void timer_fun(int timer_id, void *arg);
    
    //std::vector<timer_fn*> vec_fun;
};

#endif /* TIMER_H */

