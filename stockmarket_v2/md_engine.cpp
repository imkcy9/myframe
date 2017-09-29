/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_engine.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月12日, 下午2:48
 */

#include <zmq.h>

#include "md_engine.h"
#include "util.h"

md_engine::md_engine(zmq::context_t* ctx)
: zmq_poller_reactor(ctx)
, m_md_cllector(ctx)
, m_inner_sub(*ctx, ZMQ_SUB)
, m_stcoe_updator(0)
#ifdef USE_ZMQ_PUB
, m_pub(*ctx, ZMQ_PUB)
#else
, m_pub(*ctx, ZMQ_YSSTREAM)
#endif
, m_bFirstSubscribe(true) {

}

md_engine::~md_engine() {
    m_md_cllector.stop();
    if (m_stcoe_updator)
        delete m_stcoe_updator;
    m_pub.close();
    m_inner_sub.close();

}

bool md_engine::init() {
    try {
        if (!m_stcoe_updator) {
            m_stcoe_updator = new stcode_updator();
        }

        int recvhwm = 5000;
        m_inner_sub.setsockopt(ZMQ_SUBSCRIBE, "", 0);
        m_inner_sub.setsockopt(ZMQ_RCVHWM, &recvhwm, sizeof (recvhwm));
        m_inner_sub.connect("inproc://inner_pub");
        //m_pub.bind("inproc://inner_pub_protobuf");
        //m_pub.bind("ipc:///home/quote/ipc/test");
        m_zmq_monitor.init(m_pub,"inproc://monitor");
        m_pub.bind(config::Instance()->get_mdconfig_bind_addr());
        add_socket(&m_inner_sub, this);
        m_zmq_monitor.check_event();

        //订阅交由update_thread来决定
        //timers_add(timer_subscribe, 2000,this);
        timers_add(timer_ws_hearbeat, 15000, this);
        timers_add(timer_pub_hearbeat, 15000, this);
    } catch (zmq::error_t& e) {
        LOG_ERROR("{}, num {}", e.what(), e.num());
        return false;
    }

    return true;
}

void md_engine::before_end() {
    m_md_cllector.stop();

    m_md_cllector.join();
}

void md_engine::send_clear_signal() {
    protomessage msg;
    msg.mutable_clear_signal()->set_tradingday(timeutil::get_strtime(time(0), "%Y%m%d"));
    msg.mutable_clear_signal()->set_update_time(time(0));
    m_pub.send_message("", 0, TRADINGDAY_CHANGED, msg);
}

bool md_engine::subscribe(std::unordered_map<std::string, tick_info>* innercode) {
    if (!innercode || innercode->empty())
        return false;

    m_md_cllector.stop();
    bool first = true;
    int code_count = 0;

    std::string prefix_substr = config::Instance()->get_mdconfig_connect_addr();
    std::string substr = prefix_substr;

    for (auto it = innercode->begin(); it != innercode->end(); it++) {
        if (!first) {
            substr.append(",");
        } else {
            first = false;
        }
        //不订阅退市股票
        tick_info& tick = it->second;
        if (tick.status == 3) {
            continue;
        }
        if (tick.securitytype == "I") {
            //
            substr.append(tick.security_code).append("_zdp");
            substr.append(",");
            code_count++;
        }
        substr.append("dpmf_").append(tick.security_code);
        substr.append(",");
        substr.append(tick.security_code);
        if (code_count >= 256) {
            m_md_cllector.connect(substr.c_str());
            LOG_TRACE("Subscribe connect {}, size {}", substr.c_str(), substr.size());
            substr = prefix_substr;
            code_count = 0;
            first = true;
            usleep(100);
        }
        //LOG_INFO("subscribe {}", tick.security_code);

        code_count++;
        code_count++;
    }

    if (!substr.empty() && substr != prefix_substr) {
        m_md_cllector.connect(substr.c_str());
        LOG_TRACE("The last subscribe connect {}, size {}", substr.c_str(), substr.size());
    }

    LOG_INFO("subscription number {}", innercode->size());
    m_md_cllector.start();
    return true;
}

void md_engine::zmq_in_event(zmq::socket_t* socket) {
    zmq::message_t msg;
    m_inner_sub.recv(&msg);
    m_pub.publish(msg);
}

void md_engine::zmq_timer_event(int id_) {
    LOG_TRACE("zmq_timer_event {}", id_);
    if (id_ == timer_subscribe) {
        //删除timer
        timers_cancel(id_, this);
        std::unordered_map<std::string, tick_info>* m_innerCode = m_stcoe_updator->get_securitycode_map();
        m_pub.attach_inner_code(m_innerCode);
        if (subscribe(m_innerCode)) {
            LOG_INFO("Subscribe success, start recv market datas...");
            
        } else {
            LOG_ERROR("A subscription failure occurred, it will retry after 5 seconds.");
            timers_add(timer_subscribe, 5000, this);
        }
    }
    if (id_ == timer_ws_hearbeat) {
        m_md_cllector.check_connections_status();
        if (time(0) - m_md_cllector.get_last_time() > 15) {
            //send last tick;
            //m_pub.publish_the_last();
        }
    }
    if (id_ == timer_pub_hearbeat) {
        protomessage protomsg;
        heartbeat& hb = *protomsg.mutable_heart_beat();
        hb.set_update_time(time(0));
        m_pub.send_message("", 0, HEARTBEAT_CMD, protomsg);
        LOG_DEBUG("Pubber sends heartbeat...");
        m_zmq_monitor.check_event();
    }
    
    if(id_ == timer_send_clear_signal) {
        timers_cancel(id_,this);
        time_t dft = timeutil::get_HMS_diff_time(config::Instance()->get_mdconfig_clear_signal_time());
        LOG_INFO("Clear_signal time is {}, next send clear signal waiting time is {} seconds", config::Instance()->get_mdconfig_clear_signal_time(), dft);
        timers_add(timer_send_clear_signal, dft * 1000, this);
        LOG_INFO("After 200ms will resubscribe market data.");
        timers_add(timer_subscribe, 200, this);
    }

}

void md_engine::process_update_innercode(void* metadata) {
    if (metadata) {
        std::unordered_map<std::string, tick_info>* pmap = (std::unordered_map<std::string, tick_info>*)metadata;
        LOG_INFO("Pubber update EI map done.. size {}", pmap->size());
        m_pub.attach_inner_code(pmap);
        if (m_bFirstSubscribe) {
            timers_add(timer_subscribe, 200, this);
            
            time_t dft = timeutil::get_HMS_diff_time(config::Instance()->get_mdconfig_clear_signal_time());
            timers_add(timer_send_clear_signal, dft * 1000, this);
            
            LOG_INFO("For the first subscription,Application will subscribe marketdata after 200ms..Clear_signal time is {}, the next send clear signal waiting time is {} seconds.",
                    config::Instance()->get_mdconfig_clear_signal_time(), dft);
 
            m_bFirstSubscribe = false;
        }
    } else {
        LOG_ERROR("metadata is null...");
    }

}
