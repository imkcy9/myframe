/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_pubber.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月15日, 下午4:44
 */

#include "md_pubber.h"
#include "stcode_updator.h"
#include "depthmarketdata.pb.h"
#include "google/protobuf/util/json_util.h"

enum STOCK_DEPTHMARKET {
    E_SECURITY_CODE = 0,
    E_SECURITY_NAME,
    E_OPEN_PRICE,
    E_PRE_CLOSE_PRICE,
    E_LAST_PRICE,
    E_HIGH_PRICE,
    E_LOW_PRICE,
    E_BID_PRICE,
    E_ASK_PRICE,
    E_VOLUME,
    E_TURNOVER,
    E_BID_VOL1,
    E_BID_PRICE1,
    E_BID_VOL2,
    E_BID_PRICE2,
    E_BID_VOL3,
    E_BID_PRICE3,
    E_BID_VOL4,
    E_BID_PRICE4,
    E_BID_VOL5,
    E_BID_PRICE5,

    E_ASK_VOL1,
    E_ASK_PRICE1,
    E_ASK_VOL2,
    E_ASK_PRICE2,
    E_ASK_VOL3,
    E_ASK_PRICE3,
    E_ASK_VOL4,
    E_ASK_PRICE4,
    E_ASK_VOL5,
    E_ASK_PRICE5,

    E_DATE,
    E_TIME,
    E_STATUS
};

md_pubber::md_pubber(zmq::context_t& ctx, int type_)
: zmq::socket_t(ctx, type_)
, m_innerCode(0) {

}

md_pubber::~md_pubber() {
    if (m_innerCode) {
        delete m_innerCode;
        m_innerCode = NULL;
    }
}

void md_pubber::attach_inner_code(std::unordered_map<std::string, tick_info>* innerCode) {
    if (!innerCode || innerCode->empty()) {
        LOG_ERROR("发现致命的BUG，码表为空.");
        return;
    }

    if (m_innerCode) {
        delete m_innerCode;
        m_innerCode = NULL;
    }
    m_innerCode = innerCode;
}

bool md_pubber::publish(zmq::message_t& msg) {
    if (!m_innerCode || m_innerCode->empty()) {
        LOG_ERROR("发现致命的BUG，码表为空.");
        return false;
    }
    transform_to_protobuf(msg);
    //std::string str((char*) msg.data(), msg.size());
    //LOG_DEBUG("{}", str);
    return true;
}

void md_pubber::publish_the_last() {
    for (auto it = m_lastTickMap.begin(); it != m_lastTickMap.end(); it++) {
#ifdef USE_ZMQ_PUB
        send_message(it->second.mutable_market_data()->securitycode().c_str(),
                it->second.mutable_market_data()->securitycode().size() + 1,
                it->second);
#else
        send_message(SNAPSHOT_CMD, it->second);
#endif
    }
}

#ifdef USE_ZMQ_PUB

void md_pubber::send_message(const char* prefix, size_t prefix_len, google::protobuf::Message& message) {
    send(prefix, prefix_len + 1, ZMQ_SNDMORE);
    zmq::message_t msg(message.ByteSizeLong());
    message.SerializeToArray(msg.data(), msg.size());
    send(msg);
}
#else

void md_pubber::send_message(ushort cmd, google::protobuf::Message& message) {
    send("", 0, ZMQ_SNDMORE);
    send(&cmd, sizeof (cmd), ZMQ_SNDMORE);
    zmq::message_t msg(message.ByteSizeLong());
    message.SerializeToArray(msg.data(), msg.size());
    send(msg);
}
#endif

bool md_pubber::check_tick(std::vector<std::string>& vec) {
    size_t size = vec.size();
    if (size == 3) {
        std::vector<std::string> vec2;
        size = stcode_updator::Split(vec[0].c_str(), '_', vec2);
        if (size == 2) {
            std::string& securitycode = vec2[1];
            auto it = m_innerCode->find(securitycode);
            bool flag = vec2[0] == "dpmf";
            if (it != m_innerCode->end() && flag) {
                it->second.WP = atof(vec[1].c_str());
                it->second.NP = atof(vec[2].c_str());
            }
            return flag;
        }
    }

    if (size == 4) {
        std::vector<std::string> vec2;
        size = stcode_updator::Split(vec[0].c_str(), '_', vec2);
        if (size == 2) {
            std::string& securitycode = vec2[0];
            auto it = m_innerCode->find(securitycode);
            bool flag = vec2[1] == "zdp";
            if (it != m_innerCode->end() && flag) {
                it->second._zdp.z = atof(vec[1].c_str());
                it->second._zdp.d = atof(vec[2].c_str());
                it->second._zdp.p = atof(vec[3].c_str());
            }
            //LOG_DEBUG("zdp {} {} {}",it->second._zdp.z, it->second._zdp.d, it->second._zdp.p);
            return flag;
        }
    }
    return false;
}

void md_pubber::transform_to_protobuf(zmq::message_t& msg) {
    size_t size = msg.size();
    char* tick = (char*) msg.data();

    if (!size) {
        return;
    }

    if (tick[size] == '\0') {
        //把结尾换成\n便于字符串分割
        tick[size] = '\n';
    }
    CurlHelp strhelp;
    strhelp.set_string((char*) string(tick, size).c_str());
    std::string outstring;
    while (strhelp.get_next_line(outstring)) {
        std::vector<std::string> vec;
        size_t size = stcode_updator::Split(outstring.c_str(), ',', vec);
        if (check_tick(vec)) {
            //LOG_DEBUG("{}",outstring);
            continue;
        }
        if (size != 34) {
            //LOG_DEBUG("size != 34 {}",outstring);
            continue;
        }


        protomessage pmes;

        depthmarketdata& tick = *(pmes.mutable_market_data());

        tick.set_securitycode(vec[E_SECURITY_CODE]);
        tick.set_securityname(vec[E_SECURITY_NAME]);
        tick.set_openprice(atof(vec[E_OPEN_PRICE].c_str()));
        tick.set_precloseprice(atof(vec[E_PRE_CLOSE_PRICE].c_str()));
        tick.set_lastprice(atof(vec[E_LAST_PRICE].c_str()));
        tick.set_highprice(atof(vec[E_HIGH_PRICE].c_str()));
        tick.set_lowprice(atof(vec[E_LOW_PRICE].c_str()));
        tick.set_bidprice(atof(vec[E_BID_PRICE].c_str()));
        tick.set_askprice(atof(vec[E_ASK_PRICE].c_str()));
        tick.set_volume(atol(vec[E_VOLUME].c_str()));
        tick.set_turnover(atof(vec[E_TURNOVER].c_str()));

        tick.add_bp(atof(vec[E_BID_PRICE1].c_str()));
        tick.add_bp(atof(vec[E_BID_PRICE2].c_str()));
        tick.add_bp(atof(vec[E_BID_PRICE3].c_str()));
        tick.add_bp(atof(vec[E_BID_PRICE4].c_str()));
        tick.add_bp(atof(vec[E_BID_PRICE5].c_str()));

        tick.add_bv(atol(vec[E_BID_VOL1].c_str()));
        tick.add_bv(atol(vec[E_BID_VOL2].c_str()));
        tick.add_bv(atol(vec[E_BID_VOL3].c_str()));
        tick.add_bv(atol(vec[E_BID_VOL4].c_str()));
        tick.add_bv(atol(vec[E_BID_VOL5].c_str()));

        tick.add_sp(atof(vec[E_ASK_PRICE1].c_str()));
        tick.add_sp(atof(vec[E_ASK_PRICE2].c_str()));
        tick.add_sp(atof(vec[E_ASK_PRICE3].c_str()));
        tick.add_sp(atof(vec[E_ASK_PRICE4].c_str()));
        tick.add_sp(atof(vec[E_ASK_PRICE5].c_str()));

        tick.add_sv(atol(vec[E_ASK_VOL1].c_str()));
        tick.add_sv(atol(vec[E_ASK_VOL2].c_str()));
        tick.add_sv(atol(vec[E_ASK_VOL3].c_str()));
        tick.add_sv(atol(vec[E_ASK_VOL4].c_str()));
        tick.add_sv(atol(vec[E_ASK_VOL5].c_str()));

        tick.set_date(vec[E_DATE]);
        tick.set_time(vec[E_TIME]);
        tick.set_status((vec[E_STATUS]));


        string updTime = vec[E_DATE] + " " + vec[E_TIME];
        time_t timeValue = 0;

        struct tm tmInfo;
        memset(&tmInfo, 0, sizeof (tmInfo));
        if (NULL != strptime(updTime.c_str(), "%Y-%m-%d %H:%M:%S", &tmInfo)) {
            timeValue = mktime(&tmInfo);
        }

        tick.set_updtime(timeValue);
        //tick.set_turnoverrate()


        std::unordered_map<std::string, tick_info>::iterator it = m_innerCode->find(vec[E_SECURITY_CODE]);
        if (it != m_innerCode->end()) {
            tick.set_ei(it->second.ei);
            tick.set_volume(tick.volume() * it->second.vol_multiple);
            tick.set_turnoverrate(tick.volume() / ((double) it->second.circle_share * 10000));
            tick.set_avpri(tick.turnover() / (double) tick.volume());
            tick.set_sa((tick.highprice() - tick.lowprice()) / tick.precloseprice());
            tick.set_tradvol(it->second.last_vol == 0 ? 0 : tick.volume() - it->second.last_vol);
            tick.set_wp(it->second.WP);
            tick.set_np(it->second.NP);
            tick.set_z(it->second._zdp.z);
            tick.set_d(it->second._zdp.d);
            tick.set_p(it->second._zdp.p);
            tick.set_totstock((double) it->second.total_share);
            tick.set_cirstock((double) it->second.circle_share);
            it->second.ws_ticker = outstring;
            //保存最后一条
            m_lastTickMap[tick.securitycode()] = pmes;
            it->second.last_vol = tick.volume();
            //忽略第一次行情
            //if (it->second.isFirstTick) {
            //it->second.isFirstTick = false;
            //continue;
            //}

#ifdef USE_ZMQ_PUB
            send_message(tick.securitycode().c_str(), tick.securitycode().size() + 1, pmes);
#else
            send_message(SNAPSHOT_CMD, pmes);
#endif
            //std::string out;
            //google::protobuf::util::MessageToJsonString(tick,&out);
            //LOG_DEBUG("{}",out);

        } else {
            LOG_ERROR("【未能找到合约对应的内码】:{}", tick.securitycode().c_str());
            return;
        }
        //AC_DEBUG("【推送服务推送成功】:%s, %d", buffer.c_str(), ++count);
    }
    //LOG_DEBUG("end");
}
