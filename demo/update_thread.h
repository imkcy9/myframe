/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   update_thread.h
 * Author: chengyeke
 *
 * Created on 2017年9月20日, 下午4:16
 */

#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H
#include "zmq_poller_reactor.h"
#include "zmq_ystech_msg_dispatcher.h"

class update_thread : public zmq_ystech_msg_dispatcher<update_thread>, public zmq_poller_reactor {
public:
    update_thread(zmq::context_t* ctx);
    virtual ~update_thread();

    bool init();

    void zmq_timer_event(int id_) override;

    int on_recv_tick(ushort cmd, zmq::message_t& msg, zmq::message_t& rid);
    int on_recv_hb(ushort cmd, zmq::message_t& msg, zmq::message_t& rid);
private:
    enum timer_id {
        timer_test = 0,
        timer_test2,
        timer_test3
    };
    bool before_start() override;

    void before_end() override;

    zmq::socket_t m_sock;
};

#endif /* UPDATE_THREAD_H */

