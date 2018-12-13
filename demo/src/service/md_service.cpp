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
kt::md_service::md_service(orcfix_msg_dispatcher* call_back, CXApi** md)
:base_service(sd_constants_t::Services::MarketDataService, call_back)
,_md(md)
{
}

kt::md_service::~md_service() {
}

void kt::md_service::handle_message(kt::sd_message_t& sd_message_, kt::user& user_) {
    if(sd_message_.GetMessage_tag() == sd_constants_t::MessageTypes::CreateOrderReq) {
        field_struct_t* fstruct = sd_message_.get_value(sd_constants_t::fields.Order)->GetField_struct();
        std::string orderBookId = fstruct->get_string(sd_constants_t::fields.OrderBookId);
        LOG_DEBUG("handle_message {}", orderBookId);
    }

    switch (sd_message_.GetMessage_tag()) {
        case sd_constants_t::MessageTypes::TopOfBookSubscriptionControlReq:
            on_topofbook_subscription(sd_message_, user_);
            break;
        case sd_constants_t::MessageTypes::DepthSubscriptionControlReq:
            on_depth_subscription(sd_message_, user_);
            break;
        case sd_constants_t::MessageTypes::RFQSubscriptionControlReq:
            on_rfq_subscription(sd_message_, user_);
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
    _topofbook_subscriptions.remove(user_);
    _depth_subscriptions.remove(user_);
    _rfq_subscriptions.remove(user_);
}

void kt::md_service::on_rsp_qry_instrument(InstrumentField* pInstrument, bool bIsLast) {
    //LOG_DEBUG("on_rsp_qry_instrument {} {} {}", pInstrument->InstrumentName, pInstrument->Symbol, bIsLast);
}

void kt::md_service::OnRtnDepthMarketDataN(DepthMarketDataNField* pMarketData) {
    kt::sd_message_t sd_msg(kt::sd_constants_t::MessageTypes::TopOfBookFeed, this->get_service_tag(), kt::sd_constants_t::Subscriptions::TopOfBookSubscription);
    std::string symbol = pMarketData->InstrumentID;
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
//    for(auto user : _connected_users) {
//        kt::user& user_ = user.second;
//        send(sd_msg ,user_);
//    }
    const std::set<CLIENT>& clients = _topofbook_subscriptions.get_subscriptions(symbol);
    for(CLIENT client : clients) {
        send(sd_msg, client);
    }
    
    auto it = _marketdata_cache.find(symbol);
    if(it == _marketdata_cache.end()) {
        _marketdata_cache.insert(std::make_pair(symbol, sd_msg));
    } else {
        it->second = sd_msg;
    }
    //LOG_DEBUG("send {} users", _connected_users.size());
    
    
    //build depth marketdata
    sd_message_t depth_msg(sd_constants_t::MessageTypes::DepthFeed, this->get_service_tag(), kt::sd_constants_t::Subscriptions::DepthSubscription);
    depth_msg.add(sd_constants_t::fields.OrderBookId, symbol);
    std::vector<field_struct_t> bid_level_structs;
    for(int i = 0; i < 5; i++) {
        field_struct_t levelStruct(false);
        levelStruct.add(sd_constants_t::fields.MDEntryId, "MDEntryId");
        levelStruct.add(sd_constants_t::fields.MdEntryPositionNo, (long)i);
        levelStruct.add(sd_constants_t::fields.MdUpdateAction, '1');
        levelStruct.add(sd_constants_t::fields.Price, (double)(5-1));
        levelStruct.add(sd_constants_t::fields.OrderQty, (double)(i+1));
        levelStruct.add(sd_constants_t::fields.Side, '1');
        bid_level_structs.push_back(levelStruct);
    }
    
    std::vector<field_struct_t> ask_level_structs;
    for(int i = 0; i < 5; i++) {
        field_struct_t levelStruct(false);
        levelStruct.add(sd_constants_t::fields.MDEntryId, "MDEntryId");
        levelStruct.add(sd_constants_t::fields.MdEntryPositionNo, (long)i);
        levelStruct.add(sd_constants_t::fields.MdUpdateAction, '1');
        levelStruct.add(sd_constants_t::fields.Price, (double)(10-1));
        levelStruct.add(sd_constants_t::fields.OrderQty, (double)(i+1));
        levelStruct.add(sd_constants_t::fields.Side, '0');
        ask_level_structs.push_back(levelStruct);
    }
    
    std::vector<field_value_t> depthLevelValues;
    
    //depthLevelValues.resize(bid_level_structs.size() + ask_level_structs.size());
    for(field_struct_t& fieldStruct : bid_level_structs) {
        depthLevelValues.push_back(field_value_t::value_of(&fieldStruct));
    }
    for(field_struct_t& fieldStruct : ask_level_structs) {
        depthLevelValues.push_back(field_value_t::value_of(&fieldStruct));
    }
    

    depth_msg.add(sd_constants_t::fields.DepthLevels, field_value_t::value_of(depthLevelValues));
//    for(auto user : _connected_users) {
//        kt::user& user_ = user.second;
//        send(depth_msg ,user_);
//    }
    
    const std::set<CLIENT>& dclients = _depth_subscriptions.get_subscriptions(symbol);
    for(CLIENT client : dclients) {
        send(depth_msg, client);
    }
    
    it = _depth_marketdata_cache.find(symbol);
    if(it == _depth_marketdata_cache.end()) {
        _depth_marketdata_cache.insert(std::make_pair(symbol, depth_msg));
    } else {
        it->second = depth_msg;
    }
}

void kt::md_service::on_depth_subscription(kt::sd_message_t& sd_message_, kt::user& user_) {
    std::string orderbookid = sd_message_.get_string(sd_constants_t::fields.OrderBookId);
    char type = sd_message_.get_value(sd_constants_t::fields.SubscriptionRequestType)->GetInt8_value();
    int depth = sd_message_.get_value(sd_constants_t::fields.MarketDepth)->GetInt32_value();
    LOG_DEBUG("handle_depth_subscription {}, reqType {}, depth {}", orderbookid, type, depth);
}

void kt::md_service::on_rfq_subscription(kt::sd_message_t& sd_message_, kt::user& user_) {
    LOG_DEBUG("handle_rfq_subscription ");
}

void kt::md_service::on_topofbook_subscription(kt::sd_message_t& sd_message_, kt::user& user_) {
    std::string orderbookid = sd_message_.get_string(sd_constants_t::fields.OrderBookId);
    char type = sd_message_.get_value(sd_constants_t::fields.SubscriptionRequestType)->GetInt8_value();
    switch(type) {
        case '0':
            handle_snapshot(orderbookid, user_);
            break;
        case '1':
            handle_subscribe(orderbookid, user_);
            break;
        case '2':
            handle_unsubscribe(orderbookid, user_);
            break;
        default:
            assert(false);
            break;
    }
    LOG_DEBUG("handle_topofbood_subscription {}, reqType {}", orderbookid, type);
}

void kt::md_service::handle_depth_snapshot(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_depth_subscribe(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_depth_unsubscribe(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_rfq_snapshot(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_rfq_subscribe(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_rfq_unsubscribe(std::string orderbookid_, kt::user& user_) {

}

void kt::md_service::handle_snapshot(std::string orderbookid_, kt::user& user_) {
    for(auto& msg : _marketdata_cache) {
        this->send(msg.second, user_);
    }
}

void kt::md_service::handle_subscribe(std::string orderbookid_, kt::user& user_) {
    bool ret = _topofbook_subscriptions.add(user_, orderbookid_);
    if(!ret) {
        LOG_WARN("client {} already subscribe {}", user_.to_string(), orderbookid_);
        return;
    }
    
    auto it = this->_marketdata_cache.find(orderbookid_);
    size_t size = _marketdata_cache.size();
    if(it != _marketdata_cache.end()) {
        this->send(it->second, user_);
    }
    
    (*_md)->Subscribe(orderbookid_.c_str(), "");
}

void kt::md_service::handle_unsubscribe(std::string orderbookid_, kt::user& user_) {
    bool ret = _topofbook_subscriptions.remove(user_, orderbookid_);
    if(ret) {
        //(*_md)->Unsubscribe(orderbookid_.c_str(), "");
    }
}
