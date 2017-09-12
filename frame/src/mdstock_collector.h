/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mdstock_collector.h
 * Author: chengyeke
 *
 * Created on 2017年9月11日, 下午12:50
 */

#ifndef MDSTOCK_COLLECTOR_H
#define MDSTOCK_COLLECTOR_H
#include "log.h"
#include <uWS/uWS.h>
#include "thread.h"
#include <zmq.hpp>

class mdstock_collector : public thread {
public:
    mdstock_collector(zmq::context_t* ctx);
    
    virtual ~mdstock_collector();
    
    void connect(const char* addr);
    
    void run() override;

    
    void stop();

    void check_connections_status();
    
private:

    void start_md_collector();
    uWS::Hub h;
    zmq::socket_t m_pub;
};

#endif /* MDSTOCK_COLLECTOR_H */

