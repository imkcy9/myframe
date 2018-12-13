/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sd_constants.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午5:59
 */

#ifndef SD_CONSTANTS_H
#define SD_CONSTANTS_H
#include "field_tag.h"

namespace kt {

    class sd_constants_t {
    public:

        enum Services {
            InstrumentService = 1,
            MarketDataService = 2,
            OrderService = 3,
            MassQuoteService = 5,
            TradeReportService = 6
        };

        enum MessageTypes {
            // Login
            LoginReq = 65,
            LoginRsp = 97,

            // Logout
            LogoutReq = 66,
            LogoutRsp = 98,

            // Generic
            SnapshotEndResponse = 46,

            // Instrument service
            FindInstrumentsReq = 105,
            InstrumentFeed = 50,
            InstrumentLegFeed = 54,
            OrderBookFeed = 52,
            PublicTradesSubscriptionControlReq = 80,
            InstrumentsSubscriptionControlReq = 49,
            ListTickRuleTablesReq = 51,
            TickRulesTableFeed = 116,
            TickRuleTablesSubscriptionControlReq = 55,
            OrderBooksSubscriptionControlReq = 38,

            // Market Data Service
            TopOfBookSubscriptionControlReq = 70,
            TopOfBookFeed = 102,
            NewsSubscriptionControlReq = 57,
            DepthSubscriptionControlReq = 68,
            DepthFeed = 100,
            RfqSubscriptionControlRequest = 82,
            RfqFeed = 114,

            // Order service
            CreateOrderReq = 76,
            CreateOrderRsp = 108,
            OrdersSubscriptionControlReq = 79,
            TradesSubscriptionControlReq = 88,
            KeepAliveReq = 34,
            ModifyOrderReq = 71,
            CancelOrderReq = 77,
            CancelOrderRsp = 109,
            FlushReq = 35,
            ModifyOrderRsp = 103,
            OrderFeed = 111,
            TradeFeed = 120,
            CreateInstrumentReq = 53,
            RFQSubscriptionControlReq = 82,
            AdvanceTradeSubscriptionControlReq = 63,
            OrdersSnapshotEndRsp = 61,
            EnterRfqRequest = 85,
            EnterRfqResponse = 117,
            MassCancelOrdersRequest = 47,

            // Mass quote service
            EnterMassQuoteReq = 83,
            EnterMassQuoteRsp = 115,
            CancelMassQuoteReq = 75,
            CancelMassQuoteRsp = 107,
            CancelAllMassQuotesReq = 126,
            CancelAllMassQuotesRsp = 94,
            QuotesSubscriptionControlReq = 74,
            QuotesSnapshotEndRsp = 59,
            QuoteFeed = 106,
            QuoteProtectionSubscriptionControlReq = 91,
            GetResourceRequest = 129,
            GetResourceResponse = 130,

            // Trade report
            EnterTradeReportRequest = 89,
            EnterTradeReportResponse = 121
        };

        struct Fields {

            void init() {
                Username = field_tag_t::value_of("Username");
                RequestId = field_tag_t::value_of("RequestId");
                ServiceVersion = field_tag_t::value_of("ServiceVersion");
                RejectReason = field_tag_t::value_of("RejectReason");
                OrderRejectReason = field_tag_t::value_of("OrderRejectReason");
                Instrument = field_tag_t::value_of("Instrument");
                ApplicationVersion = field_tag_t::value_of("ApplicationVersion");
                ServiceImplVersion = field_tag_t::value_of("ServiceImplVersion");
                MessageEncoding = field_tag_t::value_of("MessageEncoding");
                TransactionId = field_tag_t::value_of("TransactionId");
                SubscriptionRequestType = field_tag_t::value_of("SubscriptionRequestType");
                MessageTypeId = field_tag_t::value_of("MessageTypeId");
                Group = field_tag_t::value_of("Group");
                AuthenticationType = field_tag_t::value_of("AuthenticationType");
                LoginData = field_tag_t::value_of("LoginData");
                SignatureChallenge = field_tag_t::value_of("SignatureChallenge");
                AuthenticationResponse = field_tag_t::value_of("AuthenticationResponse");

                // OrderBookFields group
                OrderBooks = field_tag_t::value_of("OrderBooks");
                OrderBookId = field_tag_t::value_of("OrderBookId");
                TickRulesTableId = field_tag_t::value_of("TickRulesTableId");
                Currency = field_tag_t::value_of("Currency");
                RoundLot = field_tag_t::value_of("RoundLot");
                PriceMultiplier = field_tag_t::value_of("PriceMultiplier");
                MassQuoteId = field_tag_t::value_of("MassQuoteId");

                // InstrumentIDs group
                Symbol = field_tag_t::value_of("Symbol");

                // Instrument
                CFICode = field_tag_t::value_of("CfiCode");
                Product = field_tag_t::value_of("Product");
                SecurityType = field_tag_t::value_of("InstrumentType");
                InstrumentDescription = field_tag_t::value_of("InstrumentDescription");
                Mic = field_tag_t::value_of("Mic");
                ContractMultiplier = field_tag_t::value_of("ContractMultiplier");
                OldContractMultiplier = field_tag_t::value_of("OldContractMultiplier");
                UnderlyingSymbol = field_tag_t::value_of("UnderlyingSymbol");
                TickRules = field_tag_t::value_of("TickRules");
                StartTickPriceRange = field_tag_t::value_of("StartTickPriceRange");
                TickIncrement = field_tag_t::value_of("TickIncrement");
                MaturityDate = field_tag_t::value_of("MaturityDate");
                StrikePrice = field_tag_t::value_of("StrikePrice");
                PriceDisplay = field_tag_t::value_of("PriceDisplay");
                StrikeCurrency = field_tag_t::value_of("StrikeCurrency");
                ExerciseStyle = field_tag_t::value_of("ExerciseStyle");

                // Market data
                OrderBook = field_tag_t::value_of("OrderBook");
                TradingStatus = field_tag_t::value_of("TradingStatus");
                FeedStatus = field_tag_t::value_of("FeedStatus");
                BidPrice = field_tag_t::value_of("BidPrice");
                BidSize = field_tag_t::value_of("BidQuantity");
                ImpliedBidQuantity = field_tag_t::value_of("ImpliedBidQuantity");
                OfferPrice = field_tag_t::value_of("OfferPrice");
                OfferSize = field_tag_t::value_of("OfferQuantity");
                ImpliedOfferQuantity = field_tag_t::value_of("ImpliedOfferQuantity");
                BidAtMarket = field_tag_t::value_of("BidAtMarket");
                OfferAtMarket = field_tag_t::value_of("OfferAtMarket");
                ExchangeModificationTimestamp = field_tag_t::value_of("ExchangeModificationTimestamp");
                ExchangeCreationTimestamp = field_tag_t::value_of("ExchangeCreationTimestamp");
                DepthLevel = field_tag_t::value_of("DepthLevel");
                DepthLevels = field_tag_t::value_of("DepthLevels");
                MDEntryId = field_tag_t::value_of("MdEntryId");
                MdEntryPositionNo = field_tag_t::value_of("MdEntryPositionNo");
                MdUpdateAction = field_tag_t::value_of("MdUpdateAction");
                MDEntryOriginator = field_tag_t::value_of("MdEntryOriginator");
                NumberOfOrders = field_tag_t::value_of("NumberOfOrders");
                Priority = field_tag_t::value_of("Priority");
                OpeningPrice = field_tag_t::value_of("OpeningPrice");
                ClosingPrice = field_tag_t::value_of("ClosingPrice");
                MarketDepth = field_tag_t::value_of("MarketDepth");

                // Transaction subscriptions
                Owner = field_tag_t::value_of("Owner");
                NumberOfDays = field_tag_t::value_of("NumberOfDays");

                // Order
                Order = field_tag_t::value_of("Order");
                OrderId = field_tag_t::value_of(5066);
                Side = field_tag_t::value_of("Side");
                OrderQty = field_tag_t::value_of("OrderQuantity");
                OrdType = field_tag_t::value_of("OrderType");
                TimeInForce = field_tag_t::value_of("TimeInForce");
                Price = field_tag_t::value_of("Price");
                Originator = field_tag_t::value_of("Originator");
                OrdRejReason = field_tag_t::value_of("OrderRejectReason");
                OrdStatus = field_tag_t::value_of("OrderStatus");
                Text = field_tag_t::value_of("Text");
                Reason = field_tag_t::value_of("Reason");
                FilledQuantity = field_tag_t::value_of("FilledQuantity");
                CancelledQuantity = field_tag_t::value_of("CancelledQuantity");
                MarketOrderId = field_tag_t::value_of("MarketOrderId");
                CxlRejReason = field_tag_t::value_of("CancelRejectReason");
                RetainFlag = field_tag_t::value_of("RetainFlag");
                ResourceId = field_tag_t::value_of("ResourceId");
                OpaqueData = field_tag_t::value_of("OpaqueData");
                Rfq = field_tag_t::value_of("Rfq");
                QuoteRequestId = field_tag_t::value_of("QuoteRequestId");
                InternalAccount = field_tag_t::value_of("InternalAccount");

                // Trade
                Trade = field_tag_t::value_of("Trade");
                TradeId = field_tag_t::value_of("TradeId");
                LastPrice = field_tag_t::value_of("LastPrice");
                LastQuantity = field_tag_t::value_of("LastQuantity");
                MarketTradeId = field_tag_t::value_of("MarketTradeId");
                SecondaryOrderId = field_tag_t::value_of("SecondaryOrderId");

                // Mass quote
                QuoteEntries = field_tag_t::value_of("QuoteEntries");
                BidQuoteSide = field_tag_t::value_of("BidQuoteSide");
                QuoteId = field_tag_t::value_of("QuoteId");
                MaxQuoteID = field_tag_t::value_of(117);

                OfferQuoteSide = field_tag_t::value_of("OfferQuoteSide");
                QuoteStatus = field_tag_t::value_of("QuoteStatus");
                QuoteStatusReason = field_tag_t::value_of("QuoteStatusReason");

                // Trade report
                TradeReport = field_tag_t::value_of("TradeReport");
                TradeReportKind = field_tag_t::value_of("TradeReportKind");
                TradeReportTransactionType = field_tag_t::value_of("TradeReportTransactionType");
                TradeReportType = field_tag_t::value_of("TradeReportType");
                ContraBroker = field_tag_t::value_of("ContraBroker");
            }
            field_tag_t Username;
            field_tag_t RequestId;
            field_tag_t ServiceVersion;
            field_tag_t RejectReason;
            field_tag_t OrderRejectReason;
            field_tag_t Instrument;
            field_tag_t ApplicationVersion;
            field_tag_t ServiceImplVersion;
            field_tag_t MessageEncoding;
            field_tag_t TransactionId;
            field_tag_t SubscriptionRequestType;
            field_tag_t MessageTypeId;
            field_tag_t Group;
            field_tag_t AuthenticationType;
            field_tag_t LoginData;
            field_tag_t SignatureChallenge;
            field_tag_t AuthenticationResponse;

            // OrderBookFields group
            field_tag_t OrderBooks;
            field_tag_t OrderBookId;
            field_tag_t TickRulesTableId;
            field_tag_t Currency;
            field_tag_t RoundLot;
            field_tag_t PriceMultiplier;
            field_tag_t MassQuoteId;

            // InstrumentIDs group
            field_tag_t Symbol;

            // Instrument
            field_tag_t CFICode;
            field_tag_t Product;
            field_tag_t SecurityType;
            field_tag_t InstrumentDescription;
            field_tag_t Mic;
            field_tag_t ContractMultiplier;
            field_tag_t OldContractMultiplier;
            field_tag_t UnderlyingSymbol;
            field_tag_t TickRules;
            field_tag_t StartTickPriceRange;
            field_tag_t TickIncrement;
            field_tag_t MaturityDate;
            field_tag_t StrikePrice;
            field_tag_t PriceDisplay;
            field_tag_t StrikeCurrency;
            field_tag_t ExerciseStyle;

            // Market data
            field_tag_t OrderBook;
            field_tag_t TradingStatus;
            field_tag_t FeedStatus;
            field_tag_t BidPrice;
            field_tag_t BidSize;
            field_tag_t ImpliedBidQuantity;
            field_tag_t OfferPrice;
            field_tag_t OfferSize;
            field_tag_t ImpliedOfferQuantity;
            field_tag_t BidAtMarket;
            field_tag_t OfferAtMarket;
            field_tag_t ExchangeModificationTimestamp;
            field_tag_t ExchangeCreationTimestamp;
            field_tag_t DepthLevel;
            field_tag_t DepthLevels;
            field_tag_t MDEntryId;
            field_tag_t MdEntryPositionNo;
            field_tag_t MdUpdateAction;
            field_tag_t MDEntryOriginator;
            field_tag_t NumberOfOrders;
            field_tag_t Priority;
            field_tag_t OpeningPrice;
            field_tag_t ClosingPrice;
            field_tag_t MarketDepth;

            // Transaction subscriptions
            field_tag_t Owner;
            field_tag_t NumberOfDays;

            // Order
            field_tag_t Order;
            field_tag_t OrderId;
            field_tag_t Side;
            field_tag_t OrderQty;
            field_tag_t OrdType;
            field_tag_t TimeInForce;
            field_tag_t Price;
            field_tag_t Originator;
            field_tag_t OrdRejReason;
            field_tag_t OrdStatus;
            field_tag_t Text;
            field_tag_t Reason;
            field_tag_t FilledQuantity;
            field_tag_t CancelledQuantity;
            field_tag_t MarketOrderId;
            field_tag_t CxlRejReason;
            field_tag_t RetainFlag;
            field_tag_t ResourceId;
            field_tag_t OpaqueData;
            field_tag_t Rfq;
            field_tag_t QuoteRequestId;
            field_tag_t InternalAccount;

            // Trade
            field_tag_t Trade;
            field_tag_t TradeId;
            field_tag_t LastPrice;
            field_tag_t LastQuantity;
            field_tag_t MarketTradeId;
            field_tag_t SecondaryOrderId;

            // Mass quote
            field_tag_t QuoteEntries;
            field_tag_t BidQuoteSide;
            field_tag_t QuoteId;
            field_tag_t MaxQuoteID;

            field_tag_t OfferQuoteSide;
            field_tag_t QuoteStatus;
            field_tag_t QuoteStatusReason;

            // Trade report
            field_tag_t TradeReport;
            field_tag_t TradeReportKind;
            field_tag_t TradeReportTransactionType;
            field_tag_t TradeReportType;
            field_tag_t ContraBroker;

        };
        static Fields fields;

        enum Operations {
            LoginOperation = 1,
            LogoutOperation = 2,
            CreateOrderOperation = 3,
            EnterMassQuoteOperation = 3,
            ModifyOrderOperation = 4,
            CancelAllMassQuotesOperation = 4,
            CancelOrderOperation = 5,
            CancelMassQuoteOperation = 5,
            MassCancelOrdersOperation = 6,
            EnterRfqOperation = 7,
            KeepAliveOperation = 15,
            GetResourceOperation = 16,
            EnterTradeReportOperation = 4
        };

        enum Subscriptions {
            InstrumentsSubscription = 4,
            TopOfBookSubscription = 3,
            TickRuleTablesSubscription = 6,
            OrderBooksSubscription = 7,
            OrdersSubscription = 8,
            TradesSubscription = 9,
            DepthSubscription = 4,
            QuoteSubscription = 7,
            QuoteProtectionOperation = 11,
            QuoteProtectionSubscription = 13,
            RfqSubscription = 7
        };
    };



}


#endif /* SD_CONSTANTS_H */

