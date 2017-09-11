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

mdstock_collector::mdstock_collector() {
}

mdstock_collector::~mdstock_collector() {
}

bool mdstock_collector::init_before_start() {
    return true;
}

void mdstock_collector::release_before_end() {

}

void mdstock_collector::run() {

    uWS::Hub& h = this->h;
    const char *broadcastMessage = "This will be broadcasted!";
    size_t broadcastMessageLength = strlen(broadcastMessage);

    int connections = 14;
    h.onConnection([&h, &connections, broadcastMessage, broadcastMessageLength](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        
    });
    
    int broadcasts = connections;
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        {
            //broadcasts--;
            LOG_INFO("{}, {}",message,ws->getFd());
        }
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
        if (code != 1000) {
            std::cout << "FAILURE: Invalid close code!" <<code << " " << message << std::endl;
            
            
        }

    });

    //h.startAutoPing(10);

    //for (int i = 0; i < connections; i++) 
    {
        h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", nullptr);
        h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", nullptr);
        h.connect("ws://hq.sinajs.cn/wskt?list=sh601006", nullptr);
    }

    h.run();
    

//    if (broadcasts != 0) {
//        std::cout << "FAILURE: Invalid amount of broadcasts received!" << std::endl;
//        exit(-1);
//    }

    std::cout << "Falling through now!" << std::endl;
}




