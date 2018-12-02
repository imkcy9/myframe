/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xspi_impl.h
 * Author: kecy
 *
 * Created on 2018年12月2日, 下午2:19
 */

#ifndef XSPI_IMPL_H
#define XSPI_IMPL_H
#include "XApiCpp.h"
#include <zmq.hpp>

class xspi_impl : public CXSpi {
public:
    xspi_impl(zmq::context_t* ctx);
    virtual ~xspi_impl();

    virtual void OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1);

    virtual void OnRtnError(CXApi* pApi, ErrorField* pError);

    virtual void OnLog(CXApi* pApi, LogField* pLog);

    virtual void OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData);

    virtual void OnRspQryInstrument(CXApi* pApi, InstrumentField* pInstrument, int size1, bool bIsLast);

    virtual void OnRspQryTradingAccount(CXApi* pApi, AccountField* pAccount, int size1, bool bIsLast);

    virtual void OnRspQryInvestorPosition(CXApi* pApi, PositionField* pPosition, int size1, bool bIsLast);

    virtual void OnRspQrySettlementInfo(CXApi* pApi, SettlementInfoField* pSettlementInfo, int size1, bool bIsLast);

    virtual void OnRspQryOrder(CXApi* pApi, OrderField* pOrder, int size1, bool bIsLast);

    virtual void OnRspQryTrade(CXApi* pApi, TradeField* pTrade, int size1, bool bIsLast);

    virtual void OnRspQryQuote(CXApi* pApi, QuoteField* pQuote, int size1, bool bIsLast);

    virtual void OnRtnOrder(CXApi* pApi, OrderField* pOrder);

    virtual void OnRtnTrade(CXApi* pApi, TradeField* pTrade);

    virtual void OnRtnQuote(CXApi* pApi, QuoteField* pQuote);

    virtual void OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest);

    virtual void OnRspQryHistoricalTicks(CXApi* pApi, TickField* pTicks, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast);

    virtual void OnRspQryHistoricalBars(CXApi* pApi, BarField* pBars, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast);

    virtual void OnRspQryInvestor(CXApi* pApi, InvestorField* pInvestor, int size1, bool bIsLast);

    virtual void OnRtnInstrumentStatus(CXApi* pApi, InstrumentStatusField* pInstrumentStatus);

    virtual bool OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument);

private:

    zmq::socket_t _rsp_push;
};

#endif /* XSPI_IMPL_H */

