/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mdstock_collector.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月11日, 下午12:50
 */

#include "mdstock_collector.h"
#include <iostream>
using namespace std;

mdstock_collector::mdstock_collector(zmq::context_t* ctx)
:m_pub(*ctx,ZMQ_PUB)
,m_last_time(0){
    int sndhwm = 5000;
    m_pub.setsockopt(ZMQ_SNDHWM,&sndhwm,sizeof(sndhwm));
    m_pub.bind("inproc://inner_pub");
}

mdstock_collector::~mdstock_collector() {
    m_pub.close();
}

void mdstock_collector::run() {

    start_md_collector();

}


void mdstock_collector::start_md_collector() {
    uWS::Hub& h = this->h;
    zmq::socket_t& pub = this->m_pub;
    time_t last_recv_time = this->m_last_time;
    const char *broadcastMessage = "This will be broadcasted!";
    size_t broadcastMessageLength = strlen(broadcastMessage);

    int connections = 14;
    h.onConnection([&h, &connections, broadcastMessage, broadcastMessageLength](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        LOG_INFO("onConnection {}",req.getUrl().toString());
    });
    
    int broadcasts = connections;
    h.onMessage([&pub,&last_recv_time](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        {
            zmq::message_t msg(length);
            memcpy(msg.data(),message, length);
            pub.send(msg);
            last_recv_time = time(0);
        }
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        if (code != 1000) {
            LOG_DEBUG("Invalid close code! {}, {}, ID{}",code,message,ws->getFd());
        }
        
        if(code != 9999) {
            LOG_WARN("onDisconnection{}",(char*)ws->getUserData());
            //ws->close(999,"closed",7);
            h.connect((char*)ws->getUserData(), (char*)ws->getUserData());
        }

    });

    LOG_INFO("websocket init sucess");
    h.run();
    

    
}


void mdstock_collector::connect(const char* addr) {
    h.connect(addr,new std::string(addr));
}

void mdstock_collector::stop() {
    h.getDefaultGroup<uWS::SERVER>().close(9999,(char*)"user closed",12);
    h.getDefaultGroup<uWS::CLIENT>().close(9999,(char*)"user closed",12);
    h.getDefaultGroup<uWS::SERVER>().terminate();
    h.getDefaultGroup<uWS::CLIENT>().terminate();
}

void mdstock_collector::check_connections_status() {
    h.getDefaultGroup<uWS::CLIENT>().broadcast("hb",3,uWS::TEXT);
}




