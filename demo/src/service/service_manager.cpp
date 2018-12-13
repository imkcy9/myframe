/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   service_manager.cpp
 * Author: kecy
 * 
 * Created on 2018年12月2日, 下午10:06
 */

#include "service_manager.h"
#include "log.h"
#include "zmq_poller_reactor.h"

kt::service_manager::service_manager(zmq::context_t* ctx, orcfix_msg_dispatcher* call_back, zmq_poller_reactor* reactor)
: _ctx(ctx)
, _rsp_pull(*ctx, ZMQ_PULL)
, _reactor(reactor)
, _md_service(NULL)
, _spi(NULL)
, _td(NULL)
, _md(NULL) {
    if (!_md_service) {
        _md_service = new md_service(call_back, &_md);
    }
} 

kt::service_manager::~service_manager() {
}

bool kt::service_manager::init() {

    _rsp_pull.bind("inproc://exchange");
    _reactor->add_socket(&_rsp_pull, this);
    if (!exchange_login()) {
        LOG_ERROR("exchange login fail..");
        return false;
    }
    _reactor->timers_add(1,5 * 1000, this);
    _reactor->timers_add(2,5 * 1000, this);
    return true;
}

void kt::service_manager::close() {
    exchange_logout();
    if (_spi) {
        delete _spi;
    }
    _rsp_pull.close();
}

void kt::service_manager::zmq_in_event(zmq::socket_t* socket) {
    zmq::message_t type;
    socket->recv(&type);
    assert(type.size() == 1 && type.more());
    const char* c = (const char*) type.data();
    zmq::message_t msg;
    socket->recv(&msg);
    if (msg.more()) {
        bool islast = true;
        int si = socket->recv(&islast, sizeof(bool));
        assert(si == sizeof(bool));
        switch (*c) {
            case ResponseType_OnRspQryInstrument:
                assert(sizeof(InstrumentField) == msg.size());
                _md_service->on_rsp_qry_instrument((InstrumentField*)msg.data(), islast);
                //_md->Subscribe(((InstrumentField*)msg.data())->InstrumentID, "");
                break;
            case ResponseType_OnRspQryTradingAccount:
            case ResponseType_OnRspQryInvestorPosition:
            case ResponseType_OnRspQrySettlementInfo:
            case ResponseType_OnRspQryOrder:
            case ResponseType_OnRspQryTrade:
            case ResponseType_OnRspQryQuote:
                assert(false);
                break;
            default:
                assert(false);
                break;
        }
    } else {
        switch (*c) {
            case ResponseType_OnRtnDepthMarketData:
                //assert(sizeof(DepthMarketDataNField) == msg.size());
                assert(((DepthMarketDataNField*)msg.data())->Size == msg.size());
                _md_service->OnRtnDepthMarketDataN((DepthMarketDataNField*)msg.data());
                break;
            default:
                assert(false);
                break;
        }
    }
}

void kt::service_manager::zmq_out_event() {

}

void kt::service_manager::zmq_timer_event(int id_) {
    LOG_DEBUG("zmq_timer_event {}", id_);
    if(id_ == 1) {
        
        ReqQueryField query_field;
        memset(&query_field, 0,sizeof(ReqQueryField));
        _td->ReqQuery(QueryType_ReqQryInstrument, &query_field);
        
        OrderIDType Out = { 0 };
		OrderField* pOrder = new OrderField();
		memset(pOrder, 0, sizeof(OrderField));
		strcpy(pOrder->InstrumentID, "SR901");
		pOrder->OpenClose = OpenCloseType::OpenCloseType_Open;
		pOrder->HedgeFlag = HedgeFlagType::HedgeFlagType_Speculation;
		pOrder->Price = 4800;
		pOrder->Qty = 1;
		pOrder->Type = OrderType::OrderType_Limit;
		pOrder->Side = OrderSide::OrderSide_Buy;
                strcpy(pOrder->OrderID, "9");
                strcpy(pOrder->OrderID, "222");
                strcpy(pOrder->LocalID, "10");
                pOrder->ReserveInt32 = 11;
                    
		//_td->SendOrder(pOrder, 1, Out);
                //_md->Subscribe("IH1812", "");
                _reactor->timers_cancel(id_, this);
    }
}

void kt::service_manager::on_sd_message(kt::sd_message_t* sd_message_, kt::user& user_) {
    switch (sd_message_->GetService_tag()) {
        case sd_constants_t::Services::MarketDataService:
            _md_service->on_message(*sd_message_, user_);
            break;
        case sd_constants_t::Services::InstrumentService:
            //
            //break;
        case sd_constants_t::Services::MassQuoteService:
            //
            //break;
        case sd_constants_t::Services::OrderService:
            //
            //break;
        case sd_constants_t::Services::TradeReportService:
            //
            //break;
        default:
            _md_service->on_message(*sd_message_, user_);
            break;
    }
    int i = 0;
}

bool kt::service_manager::exchange_login() {
    _spi = new xspi_impl(this->_ctx);
    char DLLPath1[1024] = "libCTP_Trade.so";
    char DLLPath2[1024] = "libCTP_Quote.so";

    ServerInfoField m_ServerInfo1 = {0};
    ServerInfoField m_ServerInfo2 = {0};
    UserInfoField m_UserInfo = {0};

    strcpy(m_ServerInfo1.BrokerID, "9999");
    strcpy(m_ServerInfo1.Address, "tcp://180.168.146.187:10030");

    strcpy(m_ServerInfo2.BrokerID, "9999");
    strcpy(m_ServerInfo2.Address, "tcp://180.168.146.187:10031");

    strcpy(m_UserInfo.UserID, "005890");
    strcpy(m_UserInfo.Password, "123456");
    
//        strcpy(m_ServerInfo1.BrokerID, "1010");
//    strcpy(m_ServerInfo1.Address, "tcp://114.255.82.175:41205");
//
//    strcpy(m_ServerInfo2.BrokerID, "1010");
//    strcpy(m_ServerInfo2.Address, "tcp://114.255.82.175:41213");
//
//    strcpy(m_UserInfo.UserID, "10028498");
//    strcpy(m_UserInfo.Password, "yhdr@4321");



    //while (true) {
    _td = CXApi::CreateApi(DLLPath1);
    _md = CXApi::CreateApi(DLLPath2);

    if (!_td->Init()) {
        LOG_ERROR("{}", _td->GetLastError());
        _td->Disconnect();
        return false;
    }

    if (!_md->Init()) {
        LOG_ERROR("{}", _md->GetLastError());
        _md->Disconnect();
        return false;
    }

    _td->RegisterSpi(_spi);
    _td->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
    LOG_INFO("trader Connect");

    _md->RegisterSpi(_spi);
    _md->Connect("./", &m_ServerInfo2, &m_UserInfo, 1);
    LOG_INFO("market Connect");
    //getchar();


    //getchar();
    //}
    return true;
}

void kt::service_manager::exchange_logout() {
    _td->Disconnect();
    _md->Disconnect();
    LOG_INFO("all�Disconnect");
}

void kt::service_manager::on_disconnect(kt::user user_) {
    this->_md_service->disconnect(user_);
}
