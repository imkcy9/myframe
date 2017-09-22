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
#include "stcode_updator.h"

class update_thread : public zmq_poller_reactor {
public:
    update_thread(zmq::context_t* ctx, mailbox_event* mailevent_handler);
    virtual ~update_thread();

    bool init();

    void zmq_timer_event(int id_) override;

    void stop() override;

private:
    void update_innercode(bool is_update_from_sina);

    enum timer_id {
        timer_update = 0,
        timer_first_update_from_sina,
        timer_first_update_from_mysql
    };
    void before_end() override;

    mailbox_event* m_mailevent_handler;
    stcode_updator* m_stcoe_updator;
};

#endif /* UPDATE_THREAD_H */

