/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_service.cpp
 * Author: kecy
 * 
 * Created on 2018年11月30日, 下午2:32
 */

#include "md_service.h"
#include "log.h"
#include "sd/data/sd_constants.h"
kt::md_service::md_service(orcfix_msg_dispatcher* call_back)
:base_service(sd_constants_t::Services::MarketDataService, call_back)
{
}

kt::md_service::~md_service() {
}

void kt::md_service::handle_message(kt::sd_message_t& sd_message_) {
    if(sd_message_.GetMessage_tag() == sd_constants_t::MessageTypes::CreateOrderReq) {
        field_struct_t* fstruct = sd_message_.get_value(sd_constants_t::fields.Order)->GetField_struct();
        std::string orderBookId = fstruct->get_string(sd_constants_t::fields.OrderBookId);
        LOG_DEBUG("handle_message {}", orderBookId);
    }
}

void kt::md_service::handle_login(kt::user user_) {
    LOG_DEBUG("handle_login {}", user_.to_string());
}

void kt::md_service::handle_logout(kt::user user_) {

}

void kt::md_service::on_rsp_qry_instrument(InstrumentField* pInstrument, bool bIsLast) {
    LOG_DEBUG("on_rsp_qry_instrument {} {}", pInstrument->InstrumentName, pInstrument->Symbol);
}

