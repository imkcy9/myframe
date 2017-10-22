/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   flow_worker.h
 * Author: chengyeke
 *
 * Created on 2017年9月21日, 下午4:22
 */

#ifndef FLOW_WORKER_H
#define FLOW_WORKER_H
#include "zmq_poller_reactor.h"
#include "file_flow/file_flow.h"

class flow_worker : public zmq_poller_reactor, public zmq_poll_events {
public:
    flow_worker(zmq::context_t* ctx);

    virtual ~flow_worker();
    
    bool init();
    
    void zmq_in_event(zmq::socket_t* socket) override;
    
    void zmq_timer_event(int id_) override;


private:
    zmq::socket_t m_sub;
    file_flow* m_fileflow;
    enum timer_event {
        timer_change_fileflow = 0
    };
    
    void create_file_flow();
    void delete_file_flow();
};

#endif /* FLOW_WORKER_H */

