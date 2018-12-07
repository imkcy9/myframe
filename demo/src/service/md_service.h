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
namespace kt {
class md_service : public base_service  {
public:
    md_service(orcfix_msg_dispatcher* call_back);
    virtual ~md_service();
    
    void handle_message(kt::sd_message_t& sd_message_) override;

    void handle_logout(kt::user user_) override;

    void handle_login(kt::user user_) override;

    void on_rsp_qry_instrument(InstrumentField* pInstrument, bool bIsLast);
    
    void OnRtnDepthMarketDataN(DepthMarketDataNField* pMarketData);
private:
    
    void handle_topofbood_subscription(kt::sd_message_t& sd_message_);
    
    void handle_rfq_subscription(kt::sd_message_t& sd_message_);
    
    void handle_depth_subscription(kt::sd_message_t& sd_message_);
};
}

#endif /* MD_SERVICE_H */

