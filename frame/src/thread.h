/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   thread.h
 * Author: kcy
 *
 * Created on 2017年8月24日, 下午10:47
 */

#ifndef THREAD_H
#define THREAD_H

class thread {
public:
    thread();
    virtual ~thread();
    void start();
    void join();

private:
    virtual void run() = 0;
    virtual bool init_before_start();
    virtual void release_before_end();
    static void worke_routine(void* arg_);
    void* thread_handler;
};

#endif /* THREAD_H */

