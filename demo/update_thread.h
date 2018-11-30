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
#include "ys_decoder.h"
#include "src/sdoff/sdoff_decoder.h"
#include "orcfix_msg_dispatcher.h"
#include "src/service/md_service.h"

namespace kt {
class update_thread : public orcfix_msg_dispatcher, public zmq_poller_reactor {
public:
    update_thread(zmq::context_t* ctx);
    virtual ~update_thread();

    bool init();

    void zmq_timer_event(int id_) override;

    void on_recv_sd_message(kt::sd_message_t* sd_message_, kt::user& user_) override;
    
    void on_disconnect(kt::user user_) override;

private:
    enum timer_id {
        timer_test = 0,
        timer_test2,
        timer_test3
    };
    bool before_start() override;

    void before_end() override;

    zmq::socket_t m_sock;
    
    md_service* _md_service;
};
}
#endif /* UPDATE_THREAD_H */

