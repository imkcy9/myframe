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
#include "ys_decoder.h"
#include "src/sd/data/sd_message.h"
#include "src/blob.h"
#include "src/sd/data/sd_constants.h"
#include "src/model/user.h"
#include "src/sdoff/sdoff_decoder.h"
#include "src/sdoff/sdoff_encoder.h"
#include <map>
typedef unsigned short ushort;
//template<class T, class D>

class orcfix_msg_dispatcher : public zmq_poll_events {
protected:
    //typedef int (T:: *msg_func) (kt::sd_message_t& sd_message_, zmq::message_t& rid);
    //    void add_message_mapping(ushort cmd, msg_func func) {
    //        _func = func;
    //    }
public:

    orcfix_msg_dispatcher()// :_func(0)
    {
        //m_decoder = new D();
    }

    virtual ~orcfix_msg_dispatcher() {
    }

    void zmq_in_event(zmq::socket_t* socket);

    virtual void on_recv_sd_message(kt::sd_message_t* sd_message_, kt::user& user_) = 0;
    virtual void on_disconnect(kt::user user_) = 0;

    virtual bool send_sd_message(kt::sd_message_t& sd_message_, kt::user& user_) = 0;

    std::string& encode(kt::sd_message_t& sd_message_) {
        return _sdoff_encoder.encode(&sd_message_);
    }
private:
    //msg_func mappingfunc;
    //msg_func next;
    std::map<kt::blob_t, kt::sdoff_decoder_t*> _clients;
    kt::sdoff_encoder _sdoff_encoder;
    //D* m_decoder;
    //msg_func _func;
};

#endif /* ZMQ_YSTECH_MSG_DISPATCHER_H */