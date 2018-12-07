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

    switch (sd_message_.GetMessage_tag()) {
        case sd_constants_t::MessageTypes::TopOfBookSubscriptionControlReq:
            handle_topofbood_subscription(sd_message_);
            break;
        case sd_constants_t::MessageTypes::DepthSubscriptionControlReq:
            handle_depth_subscription(sd_message_);
            break;
        case sd_constants_t::MessageTypes::RFQSubscriptionControlReq:
            handle_rfq_subscription(sd_message_);
            break;
        default:
            assert(false);
            break;
    }
}

void kt::md_service::handle_login(kt::user user_) {
    LOG_DEBUG("handle_login {}", user_.to_string());
}

void kt::md_service::handle_logout(kt::user user_) {

}

void kt::md_service::on_rsp_qry_instrument(InstrumentField* pInstrument, bool bIsLast) {
    //LOG_DEBUG("on_rsp_qry_instrument {} {} {}", pInstrument->InstrumentName, pInstrument->Symbol, bIsLast);
}

void kt::md_service::OnRtnDepthMarketDataN(DepthMarketDataNField* pMarketData) {
    kt::sd_message_t sd_msg(kt::sd_constants_t::MessageTypes::TopOfBookFeed, this->get_service_tag(), kt::sd_constants_t::Subscriptions::TopOfBookSubscription);
    std::string symbol = pMarketData->Symbol;
    field_struct_t order_book_struct(false);
    order_book_struct.add(sd_constants_t::fields.OrderBookId, symbol);
    
    int size = sizeof (DepthField);
    char* pBid = ((char*) pMarketData) + sizeof (DepthMarketDataNField);
    int AskCount = (pMarketData->Size - sizeof (DepthMarketDataNField)) / size - pMarketData->BidCount;
    char* pAsk = ((char*) pMarketData) + sizeof (DepthMarketDataNField) + pMarketData->BidCount*size;
    for (int i = 0; i < pMarketData->BidCount; ++i) {
        DepthField* pDF = (DepthField*) (pBid + i * size);
        order_book_struct.add(sd_constants_t::fields.BidPrice, pDF->Price);
        order_book_struct.add(sd_constants_t::fields.BidSize, (double)pDF->Size);
        //printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
    for (int i = 0; i < AskCount; ++i) {
        DepthField* pDF = (DepthField*) (pAsk + i * size);
        order_book_struct.add(sd_constants_t::fields.OfferPrice, pDF->Price);
        order_book_struct.add(sd_constants_t::fields.OfferSize, (double)pDF->Size);
        //printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
    
    order_book_struct.add(sd_constants_t::fields.LastPrice, (double)pMarketData->LastPrice);
    sd_msg.add(sd_constants_t::fields.OrderBook, order_book_struct);
    for(auto user : _connected_users) {
        kt::user& user_ = user.second;
        send(sd_msg ,user_);
    }
    //LOG_DEBUG("send {} users", _connected_users.size());
}

void kt::md_service::handle_depth_subscription(kt::sd_message_t& sd_message_) {
    std::string orderbookid = sd_message_.get_string(sd_constants_t::fields.OrderBookId);
    //int type = sd_message_.get_value(sd_constants_t::fields.SubscriptionRequestType)->GetInt8_value();
    LOG_DEBUG("handle_depth_subscription {}, reqType", orderbookid);
}

void kt::md_service::handle_rfq_subscription(kt::sd_message_t& sd_message_) {
    LOG_DEBUG("handle_rfq_subscription ");
}

void kt::md_service::handle_topofbood_subscription(kt::sd_message_t& sd_message_) {
    std::string orderbookid = sd_message_.get_string(sd_constants_t::fields.OrderBookId);
    //int type = sd_message_.get_value(sd_constants_t::fields.SubscriptionRequestType)->GetInt8_value();
    //LOG_DEBUG("handle_topofbood_subscription {}, reqType {}", orderbookid, type);
}
