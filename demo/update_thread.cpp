/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   update_thread.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月20日, 下午4:16
 */

#include <zmq.h>

#include "update_thread.h"
#include "log.h"
#include <iostream>

update_thread::update_thread(zmq::context_t* ctx)
:m_sock(*ctx,ZMQ_STREAM)
 {
    add_message_mapping(1005,&update_thread::on_recv_tick);
    add_message_mapping(1004,&update_thread::on_recv_hb);
}

update_thread::~update_thread() {

}

bool update_thread::init() {
    timers_add(timer_test, 1 * 1000, this);
    //timers_add(timer_test2, 2 * 1000, this);
    //timers_add(timer_test3, 10 * 1000, this);
    
    m_sock.setsockopt(ZMQ_CONNECT_RID,"server",6);
    m_sock.connect("tcp://192.168.19.193:59005");
    add_socket(&m_sock,this);
    return true;
}

void update_thread::before_end() {
    m_sock.close();
    LOG_INFO("update_thread end");
}

bool update_thread::before_start() {
    LOG_INFO("update_thread before_start");
    return true;
}

void update_thread::zmq_timer_event(int id_) {
    if(id_ == timer_test) {
        LOG_INFO("send hb");
        ushort cmd = 1004;
        m_sock.send("server",6,ZMQ_SNDMORE);
        NtPkgHead head;
        memset(&head,0,sizeof(NtPkgHead));
        head.wCmd = htons(1004);
        head.bStartFlag = 255;
        head.wLen = htons(20);
        //m_sock.send(&cmd,sizeof(cmd),ZMQ_SNDMORE);
        m_sock.send(&head,20);
        //timers_cancel(timer_test,this);
        
    }
    if(id_ == timer_test2) {
        LOG_INFO("timer_test2");
    }
    if(id_ == timer_test3) {
        LOG_INFO("timer_test3, stop thread");
        this->stop();
    }
}

int update_thread::on_recv_tick(ushort cmd, zmq::message_t& msg, zmq::message_t& rid) {
    //std::cout << std::string((char*)msg.data(),msg.size()) << std::endl;
    LOG_INFO("cmd {}, sid {} , msg size {}, \ntick, {}",cmd,msg.get_sid(), msg.size(), std::string((char*)msg.data(),msg.size()));
    return 0;
}

int update_thread::on_recv_hb(ushort cmd, zmq::message_t& msg, zmq::message_t& rid) {
    LOG_INFO("收到心跳  {}, {}",cmd,(char*)msg.data());
    return 0;
}
