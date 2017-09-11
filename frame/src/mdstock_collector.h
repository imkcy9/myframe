/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mdstock_collector.h
 * Author: chengyeke
 *
 * Created on 2017年9月11日, 下午12:50
 */

#ifndef MDSTOCK_COLLECTOR_H
#define MDSTOCK_COLLECTOR_H
#include "thread.h"
#include "log.h"
#include <uWS/uWS.h>


class mdstock_collector : public thread {
public:
    mdstock_collector();
    
    virtual ~mdstock_collector();

    virtual bool init_before_start();

    virtual void run();

    virtual void release_before_end();
private:
    uWS::Hub h;
};

#endif /* MDSTOCK_COLLECTOR_H */

