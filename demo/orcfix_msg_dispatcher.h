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

    void zmq_in_event(zmq::socket_t* socket) {
        ZMQ_ASSERT(socket);
        zmq::message_t rid;
        socket->recv(&rid);
        ZMQ_ASSERT(rid.more());

        zmq::message_t body;
        socket->recv(&body);
        ZMQ_ASSERT(!body.more());

        int rc = 0;
        size_t insize = body.size();
        size_t processed = 0;

        kt::blob_t routerid((const unsigned char *const) rid.data(), rid.size());
        auto it = _clients.find(routerid);
        if (insize == 0) {
            if (it == _clients.end()) {
                //connected;
                _clients.insert(std::make_pair(routerid, new kt::sdoff_decoder_t()));
            } else {
                //disconnected
                if (it->second) {
                    this->on_disconnect((reinterpret_cast<kt::sdoff_decoder_t*> (it->second))->getUser());
                    delete it->second;
                }
                _clients.erase(it);
            }
            return;
        }

        if (it == _clients.end()) {
            //
            assert(false);
            return;
        }
        const unsigned char* inpos = (const unsigned char*) body.data();
        while (insize > 0) {
            rc = it->second->decode(inpos, insize, processed);
            ZMQ_ASSERT(processed <= insize);
            inpos += processed;
            insize -= processed;
            if (rc == 0 || rc == -1)
                break;


            if (rc == 1) {
                //fix connected
                socket->send(rid, ZMQ_SNDMORE);
                socket->send("B", 1);
                return;
            }
            if (rc == 2) {
                //TODO
                //login request
                kt::sd_message_t* login_req = it->second->msg();
                std::string user_name = login_req->get_string(kt::sd_constants_t::fields.Username);
                std::string group = login_req->get_string(kt::sd_constants_t::fields.Group);
                kt::user user_(user_name, group, routerid);
                (reinterpret_cast<kt::sdoff_decoder_t*> (it->second))->setUser(user_);
                this->on_recv_sd_message(it->second->msg(), user_);
            }
            if (rc == 3) {
                this->on_recv_sd_message(it->second->msg(), it->second->getUser());
            }
            // todo, process message
            //            typename std::unordered_map<ushort,msg_func>::iterator it = m_cmdmapping.find(m_decoder->get_cmd());
            //            if(it != m_cmdmapping.end() && it->second) {
            //                //next = it->second;
            //                (static_cast <T *> (this)->*it->second)(m_decoder->get_cmd(), *m_decoder->msg(), rid);
            //            }
        }
        return;
    };

    virtual void on_recv_sd_message(kt::sd_message_t* sd_message_, kt::user& user_) = 0;
    virtual void on_disconnect(kt::user user_) = 0;

    bool send_sd_message(kt::sd_message_t& sd_message_, kt::user& user_) {
        std::string byte_buffer = _sdoff_encoder.encode(&sd_message_);
        return true;
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