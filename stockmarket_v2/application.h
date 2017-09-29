/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   application.h
 * Author: chengyeke
 *
 * Created on 2017年9月12日, 上午11:35
 */

#ifndef APPLICATION_H
#define APPLICATION_H
#include "log.h"
#include "config.h"
#include "thread.h"
#include "md_engine.h"
#include "update_thread.h"
#include "flow_worker.h"

class application : public thread_t {
public:
    application();
    virtual ~application();
    
    bool init_main(int argc, char** argv, void(sig_fun)(int));
    
    virtual void run();
    virtual bool before_start();
    virtual void before_end();
    
    void stop();
private:
    void test_sqlconnector();
    md_engine* m_pEngine;
    update_thread* m_update_thread;
    flow_worker* m_flow_worker;
    
    bool init_config();
    char g_configfilename[128];
    bool isdaemon = false;
    zmq::context_t* m_ctx;
};

#endif /* APPLICATION_H */

