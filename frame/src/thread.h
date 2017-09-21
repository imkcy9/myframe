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

class thread_t {
public:
    thread_t();
    virtual ~thread_t();
    bool init();
    void start();
    void join();

    inline bool is_running() {
        return is_start;
    }
private:
    virtual void run() = 0;
    virtual bool before_start();
    virtual void before_end();
    static void worke_routine(void* arg_);
    void* thread_handler;
    bool is_start;
};

#endif /* THREAD_H */

