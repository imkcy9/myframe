/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xspi_impl.cpp
 * Author: kecy
 * 
 * Created on 2018年12月2日, 下午2:19
 */

#include "xspi_impl.h"
#include "log.h"
#include <iostream>
#include <stdio.h>
using namespace std;

xspi_impl::xspi_impl(zmq::context_t* ctx)
: _rsp_push(*ctx, ZMQ_PUSH)
,_md_push(*ctx, ZMQ_PUSH){
    _rsp_push.connect("inproc://exchange");
//    int hwm = 10;
//    _md_push.setsockopt(ZMQ_SNDHWM,&hwm,sizeof(hwm));
    _md_push.connect("inproc://exchange");
}

xspi_impl::~xspi_impl() {
    _rsp_push.close();
}

void xspi_impl::OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1) {
    LOG_DEBUG("OnConnectionStatus {} {} {}", status, pApi->GetApiName(), pApi->GetApiTypes());
};

void xspi_impl::OnRtnError(CXApi* pApi, ErrorField* pError) {
    LOG_DEBUG("{}", pError->Text);
};

void xspi_impl::OnLog(CXApi* pApi, LogField* pLog) {
    LOG_DEBUG("{}", pLog->Message);
};

void xspi_impl::OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData) {
    //ResponseType_OnRtnDepthMarketData
    const char type = ResponseType_OnRtnDepthMarketData;
    //LOG_DEBUG("send");
    _md_push.send(&type, 1, ZMQ_SNDMORE);
    _md_push.send(pMarketData, pMarketData->Size);
    //LOG_DEBUG("done");
    
//    printf("%s,%f,%d\r\n", pMarketData->Symbol, pMarketData->LastPrice);
//
//    int size = sizeof (DepthField);
//    char* pBid = ((char*) pMarketData) + sizeof (DepthMarketDataNField);
//    int AskCount = (pMarketData->Size - sizeof (DepthMarketDataNField)) / size - pMarketData->BidCount;
//    char* pAsk = ((char*) pMarketData) + sizeof (DepthMarketDataNField) + pMarketData->BidCount*size;
//    for (int i = 0; i < pMarketData->BidCount; ++i) {
//        DepthField* pDF = (DepthField*) (pBid + i * size);
//        printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
//    }
//    for (int i = 0; i < AskCount; ++i) {
//        DepthField* pDF = (DepthField*) (pAsk + i * size);
//        printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
//    }
};

void xspi_impl::OnRspQryInstrument(CXApi* pApi, InstrumentField* pInstrument, int size1, bool bIsLast) {
    if (!pInstrument) {
        return;
    }

    const char type = ResponseType_OnRspQryInstrument;
    _rsp_push.send(&type, 1, ZMQ_SNDMORE);
    _rsp_push.send(pInstrument, sizeof (InstrumentField), ZMQ_SNDMORE);
    _rsp_push.send(&bIsLast, sizeof (bool));
    
};

void xspi_impl::OnRspQryTradingAccount(CXApi* pApi, AccountField* pAccount, int size1, bool bIsLast) {
};

void xspi_impl::OnRspQryInvestorPosition(CXApi* pApi, PositionField* pPosition, int size1, bool bIsLast) {
};

void xspi_impl::OnRspQrySettlementInfo(CXApi* pApi, SettlementInfoField* pSettlementInfo, int size1, bool bIsLast) {
};

void xspi_impl::OnRspQryOrder(CXApi* pApi, OrderField* pOrder, int size1, bool bIsLast) {
};

void xspi_impl::OnRspQryTrade(CXApi* pApi, TradeField* pTrade, int size1, bool bIsLast) {
};

void xspi_impl::OnRspQryQuote(CXApi* pApi, QuoteField* pQuote, int size1, bool bIsLast) {
};

void xspi_impl::OnRtnOrder(CXApi* pApi, OrderField* pOrder) {
    LOG_DEBUG("{} {} LocalID:{}, OrderID: {}, ID:{}, ReserveInt32:{}", pOrder->RawErrorID, pOrder->Text, pOrder->LocalID, pOrder->OrderID, pOrder->ID, pOrder->ReserveInt32);
};

void xspi_impl::OnRtnTrade(CXApi* pApi, TradeField* pTrade) {
};

void xspi_impl::OnRtnQuote(CXApi* pApi, QuoteField* pQuote) {
};

void xspi_impl::OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest) {
};

void xspi_impl::OnRspQryHistoricalTicks(CXApi* pApi, TickField* pTicks, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {
};

void xspi_impl::OnRspQryHistoricalBars(CXApi* pApi, BarField* pBars, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {
};

void xspi_impl::OnRspQryInvestor(CXApi* pApi, InvestorField* pInvestor, int size1, bool bIsLast) {
};

void xspi_impl::OnRtnInstrumentStatus(CXApi* pApi, InstrumentStatusField* pInstrumentStatus) {
};

bool xspi_impl::OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument) {
    return true;
};
