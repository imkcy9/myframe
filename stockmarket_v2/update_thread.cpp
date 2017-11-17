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

#include "update_thread.h"

update_thread::update_thread(zmq::context_t* ctx, mailbox_event* mailevent_handler)
:m_mailevent_handler(mailevent_handler) {
}

update_thread::~update_thread() {
    if (m_stcoe_updator) {
        delete m_stcoe_updator;
    }
}

bool update_thread::init() {
    if (!m_stcoe_updator) {
        m_stcoe_updator = new stcode_updator();
        if(config::Instance()->get_mdconfig_update_innercode_from_sina_before_subscribe()) {
            //update_innercode();
            
            timers_add(timer_first_update_from_sina,10,this);
        } else {
            
            timers_add(timer_first_update_from_mysql,10,this);
        }
    }
    timers_add(timer_update, 600 * 1000, this);
    timers_add(timer_tradingday_check, 10 * 1000,this);
    return true;
}

void update_thread::before_end() {

}

void update_thread::zmq_timer_event(int id_) {
    if (id_ == timer_update) {
        LOG_DEBUG("timer_update_interval");
        update_innercode(true);
    }
    if(id_ == timer_first_update_from_sina) {
        LOG_WARN("Loading EI map from Sina Web Page before subscribing marketdata..."
                " Plz set update_innercode_from_sina_before_subscribe=\"false\" if the program is surposed to subscribe marketdata right now.");
        update_innercode(true);
        timers_cancel(id_,this);
    }
    if(id_ == timer_first_update_from_mysql) {
        LOG_INFO("Loading EI map from local database before subscribing marketdata...");
        update_innercode(false);
        timers_cancel(id_,this);
    }
    if(id_ == timer_tradingday_check) {
        std::string td = m_stcoe_updator->get_tradingdday();
        if(td == "") {
            LOG_ERROR("交易日获取失败");
            return;
        }
        LOG_DEBUG("当前交易日 {} ", td);
        if(m_tradingday == "") {
            m_tradingday = td;
        } else if (m_tradingday != td && td.length() == 8) {
            m_tradingday = td;
            m_mailevent_handler->send_tradingday_changed(m_tradingday.c_str());
            LOG_DEBUG("切换交易日 {} ", td);
        }
    }
}

void update_thread::stop() {
    if (m_stcoe_updator) {
        m_stcoe_updator->stop_update();
    }
    zmq_poller_reactor::stop();
}

void update_thread::update_innercode(bool is_update_from_sina) {
    bool ret;
    if(is_update_from_sina) {
        ret = m_stcoe_updator->update_code();
    } else {
        ret = true;
    }
    
    if (ret && m_mailevent_handler) {
        m_mailevent_handler->send_update_innercode(m_stcoe_updator->get_securitycode_map());
    } else {
        LOG_WARN("码表更新被中断");
    }
}



