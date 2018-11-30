/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_decoder.h
 * Author: kecy
 *
 * Created on 2018年11月23日, 上午10:29
 */

#ifndef SDOFF_DECODER_H
#define SDOFF_DECODER_H

#include <stdlib.h>
#include <zmq.hpp>
#include "decoder.h"
#include "../model/user.h"
namespace kt {
    struct doff_head {
        char message_type;
        char protocal;
        char compression;
        char encryption;
        int16_t len;
    };

    class sdoff_decoder_t : public decoder_base_t<sdoff_decoder_t> {
    public:
        sdoff_decoder_t();
        sdoff_decoder_t(size_t bufsize_, int64_t maxmsgsize_);
        virtual ~sdoff_decoder_t();

        virtual kt::sd_message_t* msg() {
            return _sd_message;
        }
        
        kt::user& getUser() {
            return _user;
        }

        void setUser(kt::user _user) {
            this->_user = _user;
        }

    private:
        int flags_ready(unsigned char const *);
        int six_byte_size_ready(unsigned char const *);
        int message_ready(unsigned char const *);
        int sd_head_ready(unsigned char const *);
        int sd_message_ready(unsigned char const *);

        //int size_ready(uint64_t size_, unsigned char const *);

        doff_head _tmpbuf;
        char sd_head[9];
        char body[55];
        unsigned char _msg_flags;
        zmq::message_t _in_progress;
        kt::sd_message_t* _sd_message;
        const bool _zero_copy;
        const int64_t _max_msg_size;
        kt::user _user;
    };
}


#endif /* SDOFF_DECODER_H */

