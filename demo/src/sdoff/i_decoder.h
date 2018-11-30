/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_decoder.h
 * Author: kecy
 *
 * Created on 2018年11月23日, 上午9:58
 */

#ifndef I_DECODER_H
#define I_DECODER_H
#include <stdint.h>
#include <zmq.hpp>

namespace kt {
    class sd_message_t;
    class i_decoder {
    public:
        i_decoder() {}
        virtual ~i_decoder(){}

        virtual void get_buffer(unsigned char** data_, size_t* size_) = 0;
        virtual void resize_buffer (size_t) = 0;
        virtual int decode(const unsigned char *data_, size_t size_, size_t &processed_) = 0;

        virtual sd_message_t *msg() = 0;
    private:

    };
}

#endif /* I_DECODER_H */

