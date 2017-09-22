/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   flow_worker.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月21日, 下午4:22
 */

#include "flow_worker.h"
#include "depthmarketdata.pb.h"
#include "google/protobuf/util/json_util.h"
#include "log.h"
#include "config.h"
#include "util.h"
#include "md_pubber.h"

flow_worker::flow_worker(zmq::context_t* ctx)
: zmq_poller_reactor(ctx)
, m_sub(*ctx, ZMQ_YSSTREAM) {
}

flow_worker::~flow_worker() {
}

bool flow_worker::init() {
    try {
        m_sub.connect(config::Instance()->get_mdconfig_bind_addr());
        //m_sub.connect("ipc:///home/quote/ipc/test");
        add_socket(&m_sub, this);

        create_file_flow();

    } catch (zmq::error_t& e) {
        LOG_ERROR("{}", e.what());
        return false;
    }
    LOG_WARN("storage mode is on");
    return true;
}

void flow_worker::zmq_in_event(zmq::socket_t* socket) {
    assert(&m_sub == socket);
    zmq::message_t msg;
    m_sub.recv(&msg);
    assert(msg.more());
    m_sub.recv(&msg);
    uint16_t cmd = *((uint16_t*)msg.data());
    assert(msg.more());
    m_sub.recv(&msg);
    assert(!msg.more());
    //只缓存快照数据
    if(cmd == SNAPSHOT_CMD)
        m_fileflow->append(msg.data(), msg.size());
}

void flow_worker::zmq_timer_event(int id_) {
    if (id_ == timer_change_fileflow) {
        delete_file_flow();
        create_file_flow();
        timers_cancel(id_,this);
    }
}

void flow_worker::create_file_flow() {
    if (!m_fileflow) {
        std::string flow_name = "marketdata_";
        //flow_name.append(timeutil::get_strtime(time(0), "%Y%m%d"));
        flow_name.append("today");
        m_fileflow = new file_flow(flow_name.c_str(), config::Instance()->get_mdconfig_storage_path(), true);

        time_t next_waiting_time = timeutil::get_HMS_diff_time("00:01:00");
        timers_add(timer_change_fileflow, next_waiting_time*1000, this);
        LOG_INFO("The time for creating next flow file is {}",next_waiting_time);
    }
}

void flow_worker::delete_file_flow() {
    if(m_fileflow) {
        //清空截断流
        m_fileflow->truncate(0);
        delete m_fileflow;
        m_fileflow = NULL;
    }
}
