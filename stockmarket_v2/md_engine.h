/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_engine.h
 * Author: chengyeke
 *
 * Created on 2017年9月12日, 下午2:48
 */

#ifndef MD_ENGINE_H
#define MD_ENGINE_H
#include "zmq_poller_reactor.h"
#include "mdstock_collector.h"
#include <unordered_map>
#include "stcode_updator.h"
#include "md_pubber.h"
#include "zmq_poll_events.h"
#include "zmq_monitor.h"



class md_engine : public zmq_poller_reactor, public zmq_poll_events{
public:
    md_engine(zmq::context_t* ctx);
    virtual ~md_engine();
    
    bool init();
    
    bool subscribe(std::unordered_map<std::string, tick_info>* innercode);
    
    void stop() override;

    
private:
    enum timer_id {
        timer_subscribe = 0,
        timer_ws_hearbeat,
        timer_pub_hearbeat,
        timer_send_clear_signal
    };
    void zmq_in_event(zmq::socket_t* socket) override;
    
    void zmq_timer_event(int id_) override;
    
    void process_update_innercode(void* metadata) override;
    
    void process_tradingday_changed(const char* tradingday) override;
    
    void before_end() override;
    
    void send_clear_signal();
    void send_clear_signal(const char* tradingday);

    mdstock_collector m_md_cllector;
    zmq::socket_t m_inner_sub;
    
    stcode_updator* m_stcoe_updator;
    md_pubber m_pub;
    
    bool m_bFirstSubscribe;
    zmq_monitor m_zmq_monitor;
};

#endif /* MD_ENGINE_H */

