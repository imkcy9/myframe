/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_monitor.h
 * Author: chengyeke
 *
 * Created on 2017年9月27日, 下午5:52
 */

#ifndef ZMQ_MONITOR_H
#define ZMQ_MONITOR_H
#include <zmq.hpp>
#include "zookeeper_cli/zookeeper_cli.h"

class zmq_monitor : public zmq::monitor_t , public zk_event {
public:
    zmq_monitor();
    virtual ~zmq_monitor();
    

    virtual void on_event_listening(const zmq_event_t& event_, const char* addr_);

    virtual void on_event_accepted(const zmq_event_t& event_, const char* addr_);
    

    virtual void on_zookeeper_create(const char* path);

    virtual void on_zookeeper_delete(const char* path);

    virtual void on_zookeeper_changed(const char* path);


private:
    zookeeper_cli m_zkcli;
};

#endif /* ZMQ_MONITOR_H */

