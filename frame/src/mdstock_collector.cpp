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
:m_pub(*ctx,ZMQ_PUB){
    m_pub.bind("inproc://inner_pub");
}

mdstock_collector::~mdstock_collector() {
}

void mdstock_collector::run() {

    start_md_collector();

}


void mdstock_collector::start_md_collector() {
    uWS::Hub& h = this->h;
    const char *broadcastMessage = "This will be broadcasted!";
    size_t broadcastMessageLength = strlen(broadcastMessage);

    int connections = 14;
    h.onConnection([&h, &connections, broadcastMessage, broadcastMessageLength](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        LOG_INFO("onConnection {}",req.getUrl().toString());
    });
    
    int broadcasts = connections;
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        {
            //broadcasts--;
            LOG_INFO("{},{}",message,ws->getFd());
        }
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        if (code != 1000) {
            std::cout << "FAILURE: Invalid close code!" <<code << " " << message << std::endl;
            
            
        }
        
        if(code != 9999) {
            LOG_INFO("onDisconnection{}",(char*)ws->getUserData());
            //ws->close(999,"closed",7);
            std::string s = (char*)ws->getUserData();
            h.connect(  s, (void*)s.c_str());
        }

    });

    h.run();
    

    LOG_INFO("websocket init sucess");
}


void mdstock_collector::connect(const char* addr) {
    //h.getDefaultGroup<uWS::SERVER>().close();
    //h.getDefaultGroup<uWS::CLIENT>().terminate();
    //h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", nullptr);
    //h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", nullptr);
    char array[] = "ws://hq.sinajs.cn/wskt?list=sh601006";
    h.connect("ws://hq.sinajs.cn/wskt?list=sh601006",(void*)array,{{"addr",array}});
    h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", (void*)"ws://hq.sinajs.cn/wskt?list=sh601006",{{"addr","array"}});
    memset(array,0,sizeof(array));

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




