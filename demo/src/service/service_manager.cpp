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
        _md_service = new md_service(call_back);
    }
}

kt::service_manager::~service_manager() {
}

bool kt::service_manager::init() {

    _rsp_pull.connect("inproc://exchange");
    _reactor->add_socket(&_rsp_pull, this);
    if (!exchange_login()) {
        LOG_ERROR("exchange login fail..");
        return false;
    }
    _reactor->timers_add(1, 10 * 1000, this);
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
    zmq::message_t msg;
    socket->recv(&msg);
    if (msg.more()) {
        bool islast = true;
        socket->recv(&islast, sizeof(bool));
        
        const char* c = (const char*) type.data();
        switch (*c) {
            case ResponseType_OnRspQryInstrument:
                assert(sizeof(InstrumentField) == msg.size());
                _md_service->on_rsp_qry_instrument((InstrumentField*)msg.data(), islast);
                break;
            case ResponseType_OnRspQryTradingAccount:
            case ResponseType_OnRspQryInvestorPosition:
            case ResponseType_OnRspQrySettlementInfo:
            case ResponseType_OnRspQryOrder:
            case ResponseType_OnRspQryTrade:
            case ResponseType_OnRspQryQuote:

                break;
            default:
                break;
        }
    }
}

void kt::service_manager::zmq_out_event() {

}

void kt::service_manager::zmq_timer_event(int id_) {
    if(id_ == 1) {
        LOG_DEBUG("zmq_timer_event");
        ReqQueryField query_field;
        memset(&query_field, 0,sizeof(ReqQueryField));
        _td->ReqQuery(QueryType_ReqQryInstrument, &query_field);
        
        OrderIDType Out = { 0 };
		OrderField* pOrder = new OrderField();
		memset(pOrder, 0, sizeof(OrderField));
		strcpy(pOrder->InstrumentID, "IF1504");
		pOrder->OpenClose = OpenCloseType::OpenCloseType_Open;
		pOrder->HedgeFlag = HedgeFlagType::HedgeFlagType_Speculation;
		pOrder->Price = 4000;
		pOrder->Qty = 1;
		pOrder->Type = OrderType::OrderType_Limit;
		pOrder->Side = OrderSide::OrderSide_Buy;

		_td->SendOrder(pOrder, 1, Out);
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