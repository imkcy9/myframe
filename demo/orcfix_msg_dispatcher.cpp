/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "orcfix_msg_dispatcher.h"

void orcfix_msg_dispatcher::zmq_in_event(zmq::socket_t* socket) {
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
}
