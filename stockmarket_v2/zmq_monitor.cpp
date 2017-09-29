/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_monitor.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月27日, 下午5:52
 */

#include "zmq_monitor.h"
#include "log.h"
#include "config.h"
#include "common.h"


zmq_monitor::zmq_monitor() {
    m_zkcli.init(config::Instance()->get_zookeeper_zookeeper_serverip(),
            common::ZOOKEEPER_RECV_TIMEOUT,this,
            false);
}

zmq_monitor::~zmq_monitor() {
}

void zmq_monitor::register_zookeeper(const char* path, const char* value) {
    //监听此结点的状态
    bool ret = m_zkcli.exists(path);
    if(ret) {
        LOG_DEBUG( "{}  node exist... {}", path, m_zkcli.get(path));
    } else {
        LOG_DEBUG( "{}  node not exist... ", path);
    }
    ret = m_zkcli.create_node(path,
            value,
            strlen(value),
            false);

    LOG_DEBUG_IF(ret, "Create zookeeper node success... {}:{}", path, value);
    

}

void zmq_monitor::on_event_listening(const zmq_event_t& event_, const char* addr_) {
    LOG_DEBUG("Server listening on addr: {}",addr_);
    register_zookeeper(common::ZOOKEEPER_PATH,
                config::Instance()->get_zookeeper_zookeeper_register_value()
                );
}

void zmq_monitor::on_event_accepted(const zmq_event_t& event_, const char* addr_) {
    LOG_DEBUG("Server accepted : {}",addr_);
}

void zmq_monitor::on_zookeeper_create(const char* path) {
    LOG_DEBUG("on_zookeeper_create {}  {}",path,m_zkcli.get(path));
}

void zmq_monitor::on_zookeeper_delete(const char* path) {
    LOG_DEBUG("on_zookeeper_delete {}",path);
    if(strcmp(path,common::ZOOKEEPER_PATH) == 0) {
        LOG_INFO("zk server unregisted serverip {}:{}", path,config::Instance()->get_mdconfig_bind_addr());
        register_zookeeper(common::ZOOKEEPER_PATH,
                config::Instance()->get_zookeeper_zookeeper_register_value()
                );
    }

}

void zmq_monitor::on_zookeeper_changed(const char* path) {
    LOG_DEBUG("on_zookeeper_changed {}",path);
}
