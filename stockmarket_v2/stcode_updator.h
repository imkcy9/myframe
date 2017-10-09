/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stcode_updator.h
 * Author: chengyeke
 *
 * Created on 2017年9月14日, 上午10:42
 */

#ifndef STCODE_UPDATOR_H
#define STCODE_UPDATOR_H

#include "CurlHelp.h"
#include <vector>
#include <set>
#include <list>
#include "stcode.h"
#include <unordered_map>

struct tick_info {
    int64_t ei;
    std::string security_code;
    int64_t updatetime;
    int64_t last_vol;
    int64_t last_trade_vol;
    double last_tunov;
    std::string ws_ticker;
    int64_t total_share; //总股本
    int64_t circle_share; //流通股本
    double WP; //外盘
    double NP; //内盘
    bool isFirstTick; //是否第一次行情
    int vol_multiple;
    int status; //
    std::string securitytype;
    struct {
        double z;
        double d;
        double p;
    } _zdp;

    tick_info()
    : updatetime(0)
    , last_vol(0)
    , last_trade_vol(0)
    , last_tunov(0)
    , total_share(0)
    , WP(0)
    , NP(0)
    , isFirstTick(true)
    , vol_multiple(1) 
    , status(0) {
        memset(&_zdp, 0, sizeof (_zdp));
    }
};

class stcode_updator {
public:
    stcode_updator();
    virtual ~stcode_updator();
    //更新股票基础数据到数据库
    bool update_code();
    
    std::string get_tradingdday();
    
    void stop_update() {
        m_stop = true;
    }
    //从数据库获取需要订阅的股票代码
    std::unordered_map<std::string,tick_info>* get_securitycode_map();
    //std::list<stock::StCode> get_securitycode_list();

    static size_t Split(const char *str, const char seps, std::vector<std::string>& strVector);
private:
    //CurlHelp m_curlhelp;
    //融资融券

    struct rzrq {
        int rzrq;
        int sgt;
        int hgt;
    };
    rzrq getRzrq(const char* securityCode);
    //MySqlServerConnect m_MySqlServerConnect;

    void check_sqlalive();
    stcode m_stcode;
    
    bool m_stop;
};

#endif /* STCODE_UPDATOR_H */

