/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_service.h
 * Author: kecy
 *
 * Created on 2018年11月30日, 下午2:32
 */

#ifndef MD_SERVICE_H
#define MD_SERVICE_H
#include "../base_service.h"
#include "XApiCpp.h"
#include "subscription.h"
#include <unordered_map>
namespace kt {
class md_service : public base_service  {
public:
    md_service(orcfix_msg_dispatcher* call_back, CXApi** md);
    virtual ~md_service();
    
    void handle_message(kt::sd_message_t& sd_message_, kt::user& user_) override;

    void handle_logout(kt::user user_) override;

    void handle_login(kt::user user_) override;

    void on_rsp_qry_instrument(InstrumentField* pInstrument, bool bIsLast);
    
    void OnRtnDepthMarketDataN(DepthMarketDataNField* pMarketData);
private:
    
    void on_topofbook_subscription(kt::sd_message_t& sd_message_, kt::user& user_);
    
    void on_rfq_subscription(kt::sd_message_t& sd_message_, kt::user& user_);
    
    void on_depth_subscription(kt::sd_message_t& sd_message_, kt::user& user_);
    
    void handle_snapshot(std::string orderbookid_, kt::user& user_);
    
    void handle_subscribe(std::string orderbookid_, kt::user& user_);
    
    void handle_unsubscribe(std::string orderbookid_, kt::user& user_);
    
    void handle_depth_snapshot(std::string orderbookid_, kt::user& user_);
    
    void handle_depth_subscribe(std::string orderbookid_, kt::user& user_);
    
    void handle_depth_unsubscribe(std::string orderbookid_, kt::user& user_);
    
    void handle_rfq_snapshot(std::string orderbookid_, kt::user& user_);
    
    void handle_rfq_subscribe(std::string orderbookid_, kt::user& user_);
    
    void handle_rfq_unsubscribe(std::string orderbookid_, kt::user& user_);
    
    subscription_t<std::string> _topofbook_subscriptions;
    
    subscription_t<std::string> _depth_subscriptions;
    
    subscription_t<std::string> _rfq_subscriptions;
    
    std::unordered_map<std::string, kt::sd_message_t> _marketdata_cache;
    
    std::unordered_map<std::string, kt::sd_message_t> _depth_marketdata_cache;
    
    std::unordered_map<std::string, kt::sd_message_t> _rfq_cache;
    
    CXApi** _md;
};
}

#endif /* MD_SERVICE_H */

