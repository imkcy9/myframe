/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_encoder.h
 * Author: kecy
 *
 * Created on 2018年11月23日, 下午5:27
 */

#ifndef SDOFF_ENCODER_H
#define SDOFF_ENCODER_H
#include <string>
namespace kt {
class sd_message_t;
class sdoff_encoder {
public:
    sdoff_encoder();
    sdoff_encoder(const sdoff_encoder& orig);
    virtual ~sdoff_encoder();
   
    std::string& encode(kt::sd_message_t* sd_message_t);
private:
    std::string _in_progress;
    
    const long STOP_BIT = 0x80;
};
}

#endif /* SDOFF_ENCODER_H */

