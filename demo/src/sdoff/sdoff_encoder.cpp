/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_encoder.cpp
 * Author: kecy
 * 
 * Created on 2018年11月23日, 下午5:27
 */

#include "sdoff_encoder.h"
#include "sd/data/sd_message.h"

kt::sdoff_encoder::sdoff_encoder() {
}

kt::sdoff_encoder::sdoff_encoder(const sdoff_encoder& orig) {
}

kt::sdoff_encoder::~sdoff_encoder() {
}

std::string& kt::sdoff_encoder::encode(kt::sd_message_t* sd_message_) {

    return this->_in_progress;
}

