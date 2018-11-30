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
#include <netinet/in.h>
#include "log.h"
#include "sd/sd_definition_loader.h"
#include <iostream>

kt::update_thread::update_thread(zmq::context_t* ctx)
: m_sock(*ctx, ZMQ_STREAM)
, _md_service(NULL) {
    //add_message_mapping(1005,&update_thread::on_recv_tick);
    //add_message_mapping(1004,&update_thread::on_recv_hb);
}

kt::update_thread::~update_thread() {

}

bool kt::update_thread::init() {
    //timers_add(timer_test, 1 * 1000, this);
    //timers_add(timer_test2, 2 * 1000, this);
    //timers_add(timer_test3, 10 * 1000, this);

    //m_sock.setsockopt(ZMQ_CONNECT_RID,"server",6);
    //m_sock.connect("tcp://192.168.19.193:59005");
    if (!_md_service) {
        _md_service = new md_service();
    }

    m_sock.bind("tcp://*:10003");
    add_socket(&m_sock, this);
    return true;
}

void kt::update_thread::before_end() {
    m_sock.close();
    if(_md_service) {
        delete _md_service;
    }
    LOG_INFO("update_thread end");
}

bool kt::update_thread::before_start() {
    LOG_INFO("update_thread before_start");
    return kt::sd_definition_loader::init();
}

void kt::update_thread::zmq_timer_event(int id_) {
    if (id_ == timer_test) {
        LOG_INFO("send hb");
    }
    if (id_ == timer_test2) {
        LOG_INFO("timer_test2");
    }
    if (id_ == timer_test3) {
        LOG_INFO("timer_test3, stop thread");
        this->stop();
    }
}

void kt::update_thread::on_recv_sd_message(kt::sd_message_t* sd_message_, kt::user& user_) {
    switch (sd_message_->GetService_tag()) {
        case sd_constants_t::Services::MarketDataService:
            _md_service->on_message(*sd_message_, user_);
            break;
        case sd_constants_t::Services::InstrumentService:
            //
            break;
        case sd_constants_t::Services::MassQuoteService:
            //
            break;
        case sd_constants_t::Services::OrderService:
            //
            break;
        case sd_constants_t::Services::TradeReportService:
            //
            break;
        default:
            break;
    }
    int i = 0;
}

void kt::update_thread::on_disconnect(kt::user user_) {
    LOG_INFO("on_disconnect {}", user_.to_string());
}
