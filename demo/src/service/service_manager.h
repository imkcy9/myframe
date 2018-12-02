/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   service_manager.h
 * Author: kecy
 *
 * Created on 2018年12月2日, 下午10:06
 */

#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H
#include "zmq_poll_events.h"
#include "zmq_poller_reactor.h"
#include "orcfix_msg_dispatcher.h"
#include "exchange/xspi_impl.h"
#include "md_service.h"
namespace kt {

    class service_manager : public zmq_poll_events {
    public:
        service_manager(zmq::context_t* ctx, orcfix_msg_dispatcher* call_back, zmq_poller_reactor* reactor);
        virtual ~service_manager();

        bool init();

        void close();
        
        virtual void zmq_in_event(zmq::socket_t* socket);

        virtual void zmq_out_event();

        virtual void zmq_timer_event(int id_);

        void on_sd_message(kt::sd_message_t* sd_message_, kt::user& user_);
    private:
        bool exchange_login();
        void exchange_logout();
        zmq::context_t* _ctx;
        zmq::socket_t _rsp_pull;
        zmq_poller_reactor* _reactor;
        md_service* _md_service;
        xspi_impl* _spi;
        CXApi* _td;
        CXApi* _md;
    };
}
#endif /* SERVICE_MANAGER_H */

