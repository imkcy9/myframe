/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_ystech_msg_dispatcher.h
 * Author: chengyeke
 *
 * Created on 2017年11月10日, 下午5:32
 */

#ifndef ZMQ_YSTECH_MSG_DISPATCHER_H
#define ZMQ_YSTECH_MSG_DISPATCHER_H

#include "zmq_poll_events.h"
#include <unordered_map>

template<class T>
class zmq_ystech_msg_dispatcher : public zmq_poll_events {
protected:
    typedef int (T:: *msg_func) (ushort cmd, void *body, size_t body_size);
    void add_message_mapping(ushort cmd, msg_func func) {
        m_cmdmapping[cmd] = func;
    }
public:
    zmq_ystech_msg_dispatcher(){};

    virtual ~zmq_ystech_msg_dispatcher(){};
    
    void zmq_in_event(zmq::socket_t* socket) {
        ZMQ_ASSERT(socket);
        zmq::message_t routingid;
        socket->recv(&routingid);
        ZMQ_ASSERT(routingid.more());
        zmq::message_t cmd;
        socket->recv(&cmd);
        ZMQ_ASSERT(cmd.more());
        ushort ucmd = *(ushort*)cmd.data();
        zmq::message_t body;
        socket->recv(&body);
        ZMQ_ASSERT(!body.more());
        typename std::unordered_map<ushort,msg_func>::iterator it = m_cmdmapping.find(ucmd);
        if(it->second) {
            //next = it->second;
            (static_cast <T *> (this)->*it->second)(ucmd, body.data(), body.size());
        }
    };
    
    

private:
    //msg_func mappingfunc;
    //msg_func next;
    std::unordered_map<ushort,msg_func> m_cmdmapping;
};

#endif /* ZMQ_YSTECH_MSG_DISPATCHER_H */

