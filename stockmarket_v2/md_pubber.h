/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_pubber.h
 * Author: chengyeke
 *
 * Created on 2017年9月15日, 下午4:44
 */

#ifndef MD_PUBBER_H
#define MD_PUBBER_H
#include <zmq.hpp>
#include <unordered_map>
#include "depthmarketdata.pb.h"

#define HEARTBEAT_CMD    1004     //心跳包
#define SNAPSHOT_CMD     1005     //快照数据
#define INST_STATUS      1006     //合约状态
#define TRADINGDAY_CHANGED 1007   //交易日变更//清盘信号

struct tick_info;
class md_pubber : public zmq::socket_t {
public:
    md_pubber(zmq::context_t& ctx, int type_);
    virtual ~md_pubber();
    
    void attach_inner_code(std::unordered_map<std::string, tick_info>* innerCode);
    
    bool publish(zmq::message_t& msg);
    
    void publish_the_last();
    
    void send_message(const char* prefix, size_t prefix_len, ushort cmd, google::protobuf::Message& message);
private:
    bool check_tick(std::vector<std::string>& vec);
    void transform_to_protobuf(zmq::message_t& msg);
    std::unordered_map<std::string, tick_info>* m_innerCode;
    std::unordered_map<std::string, protomessage> m_lastTickMap;
};

#endif /* MD_PUBBER_H */

