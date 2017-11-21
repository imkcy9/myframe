/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stcode_updator.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月14日, 上午10:42
 */

#include "stcode_updator.h"
#include <iostream>
#include "log.h"
#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "Convert.h"
using namespace std;

stcode_updator::stcode_updator()
: m_stop(false) {
}

stcode_updator::~stcode_updator() {
}

size_t stcode_updator::Split(const char *str, const char seps, std::vector<std::string>& strVector) {
    char *pstr = (char*) str;
    char value_buffer[1024];
    char *pval = value_buffer;
    while (*pstr != '\0' && *pstr != '\n') {
        //分割‘，’和‘=’
        if (*pstr == seps || *pstr == '=') {
            *pval = '\0';
            pval = value_buffer;
            strVector.push_back(value_buffer);

        } else {
            *pval = *pstr;
            pval++;
        }

        pstr++;


    }
    *pval = '\0';
    pval = value_buffer;
    strVector.push_back(value_buffer);
    return strVector.size();
}

bool stcode_updator::update_code() {
    check_sqlalive();

    CurlHelp m_curlhelp;
    m_curlhelp.perform_to_string("http://vip.stock.finance.sina.com.cn/quotes_service/view/highlights_all_for_hq.php");
    const char sep = ',';

    std::string str;
    m_stop = false;
    while (m_curlhelp.get_next_line(str) && !m_stop) {

        std::vector<std::string> vec;
        size_t size = Split(str.c_str(), sep, vec);

        //此处应该为20，如果不是，说明请求错误
        if (size != 20) {
            LOG_ERROR("码表数据HTTP请求出错");
            return false;
        }
        for (size_t i = 0; i < vec.size(); i++) {
            //std::cout << vec[i] << endl;
        }
        try {


            //ei.get_result();
            //ei.insert(vec);

            if (vec[stcode::E_SECURITY_TYPE] != "I" && vec[stcode::E_SECURITY_TYPE] != "A") {
                continue;
            }


            sql::PreparedStatement* pre_stmt = NULL;

            //是否更新数据库插入时间的标志。如果状态，代码名称，拼音改变则设为TRUE，作为增量更新用途
            bool isUpdateDbInsertTime = false;
            rzrq rzrq_;
            sql::ResultSet* res = m_stcode.get_by_securitycode(vec[stcode::E_SECURITY_CODE]);
            if (res && res->rowsCount() == 1) {
                res->next();

                //LOG_WARN("{} 已存在", res->getString("SecurityCode").c_str());
                pre_stmt = m_stcode.get_pre_update_stmt(); //update(vec, res, isUpdateDbInsertTime);
                std::string securtiName = res->getString("SecurityName");

                if (res->getString("Status") != vec[stcode::E_STATUS] && !vec[stcode::E_STATUS].empty()) {
                    LOG_DEBUG("{}{} 状态由 {} 变成 {}", securtiName, vec[stcode::E_SECURITY_CODE], res->getString("Status").c_str(), vec[stcode::E_STATUS]);
                    isUpdateDbInsertTime = true;
                }


                if (res->getString("SecurityName").length() == 0 ||
                        res->getString("ShortName") != vec[stcode::E_SHORT_NAME] ||
                        securtiName[0] == 'N' ||
                        securtiName.compare(0, 2, "ST") == 0 ||
                        securtiName.compare(0, 3, "*ST") == 0) {
                    std::string utfName = Onv::CConvert::Gbk2Utf8((char*) m_stcode.get_security_name_by_securityCode(vec[stcode::E_SECURITY_CODE]).c_str());

                    if (!utfName.empty() && utfName != securtiName) {
                        //stcode.setSecurityName(utfName);
                        pre_stmt->setString(3, utfName);
                        isUpdateDbInsertTime = true;
                        securtiName = utfName;
                        LOG_DEBUG("获取股票代码{} 的 中文名称 {}", vec[stcode::E_SECURITY_CODE], utfName);
                    }

                    if (res->getInt("rzrq") == -1 || res->getInt("sgt") == -1 || res->getInt("hgt") == -1) {
                        //设置沪申港通标志
                        rzrq_ = getRzrq(res->getString("SecurityCode").c_str());
                    } else {
                        rzrq_.rzrq = res->getInt("rzrq");
                        rzrq_.sgt = res->getInt("sgt");
                        rzrq_.hgt = res->getInt("hgt");
                    }
                    LOG_DEBUG_IF(!utfName.empty(),"{}", utfName);
                    
                }

                m_stcode.update(vec, res, isUpdateDbInsertTime)->setString(3, securtiName);
                pre_stmt->setInt(24, rzrq_.rzrq);
                pre_stmt->setInt(25, rzrq_.sgt);
                pre_stmt->setInt(26, rzrq_.hgt);
                m_stcode.get_pre_update_stmt()->executeUpdate();
            } else {
                rzrq_ = getRzrq(vec[stcode::E_SECURITY_CODE].c_str());
                std::string utfName = Onv::CConvert::Gbk2Utf8((char*) m_stcode.get_security_name_by_securityCode(vec[stcode::E_SECURITY_CODE]).c_str());
                m_stcode.insert(vec); //->executeUpdate();
                pre_stmt = m_stcode.get_pre_insert_stmt();
                pre_stmt->setString(3, utfName);
                pre_stmt->setInt(24, rzrq_.rzrq);
                pre_stmt->setInt(25, rzrq_.sgt);
                pre_stmt->setInt(26, rzrq_.hgt);
                pre_stmt->executeUpdate();
            }
            delete res;
            //LOG_DEBUG("【《股票基础信息》存入数据库成功】{}", stcode.toJson().c_str());

        } catch (sql::SQLException& e) {
            LOG_ERROR("【《股票基础信息》存入数据库失败】系统异常：sql::SQLException {}", e.what());
        } catch (std::exception& e) {
            LOG_ERROR("【《股票基础信息》存入数据库失败】系统异常：std::exception ErrText(%s)", e.what());

        }
    }

    if (m_stop) {
        return false;
    }

    return true;
}

std::string stcode_updator::get_tradingdday() {
    std::string td;
    try {
        td = m_stcode.get_tradingday();
        
    } catch (sql::SQLException& e) {
        LOG_ERROR("【获取交易日】异常：sql::SQLException {}", e.what());
    } catch (std::exception& e) {
        LOG_ERROR("【获取交易日】异常：std::exception ErrText(%s)", e.what());

    }
    return td;
}


std::unordered_map<std::string, tick_info>* stcode_updator::get_securitycode_map() {
    auto m_innerCodeMap = new std::unordered_map<std::string, tick_info>();
    try {
        check_sqlalive();

        

        sql::ResultSet* res = m_stcode.query("select * from t_stcode WHERE EnableSubMarket !=0 and Status != 0 and (SecurityType = 'I' or SecurityType = 'A')");
        while (res && res->next()) {
            tick_info tick_info_;
            tick_info_.ei = res->getInt64("ID");
            string security_code = res->getString("SecurityCode");
            tick_info_.security_code = security_code;
            tick_info_.last_trade_vol = 0;
            tick_info_.last_vol = 0;

            //总股本数据库单位是 万股
            tick_info_.total_share = res->getInt64("TotalShare");
            tick_info_.circle_share = res->getInt64("CirculatedShare");
            if (res->getString("SecurityType") == "I" && security_code.compare(0, 2, "sh") == 0) {
                //上证的指数乖以100, 深证不需要处理
                tick_info_.vol_multiple = 100;
            }
            tick_info_.status = res->getInt("Status");
            tick_info_.securitytype = res->getString("SecurityType");
            //m_innerCodeMap[security_code] = code_it->getID();
            (*m_innerCodeMap)[security_code] = tick_info_;
        }

        if(res) {
            delete res;
        }
    } catch (sql::SQLException& e) {
        LOG_ERROR("get_securitycode_map fail..{}", e.what());
    } catch (std::exception& e) {
        LOG_ERROR("get_securitycode_map fail..：std::exception ErrText({})", e.what());

    }

    return m_innerCodeMap;
}

void stcode_updator::check_sqlalive() {
    m_stcode.keep_connected();
}

stcode_updator::rzrq stcode_updator::getRzrq(const char* securityCode) {
    CurlHelp m_curlhelp;
    string url = "https://touzi.sina.com.cn/api/openapi.php/TzyFreeService.getAData?symbol=";
    url.append(securityCode);
    m_curlhelp.perform_to_string(url.c_str());

    std::string result;
    while (m_curlhelp.get_next_line(result)) {
        rapidjson::Document document;
        if (document.Parse(result.c_str()).HasParseError()) {
            LOG_ERROR("【《融资融券》】解析股票(代码:{})的融资融券信息(url:{})失败，不是JSON格式", securityCode, url.c_str());
            continue;
        }
        if (!document.IsObject() || !document.HasMember("result") || !document["result"].IsObject()) {
            LOG_ERROR("【《融资融券》】解析股票(代码:{})的融资融券信息(url:{})失败，不能正确解析到result字段", securityCode, url.c_str());
            continue;
        }
        const rapidjson::Value& resultValue = document["result"];
        if (!resultValue.HasMember("data") || !resultValue["data"].IsObject()) {
            LOG_ERROR("【《融资融券》】解析股票(代码:{})的公告融资融券(url:{})失败，不能正确解析到data字段", securityCode, url.c_str());
            continue;
        }
        const rapidjson::Value& dataValue = resultValue["data"];
        if (dataValue.HasMember("rzrq") && dataValue.HasMember("sgt") && dataValue.HasMember("hgt")) {
            return {dataValue["rzrq"].GetInt(), dataValue["sgt"].GetInt(), dataValue["hgt"].GetInt()};
        }
    }
    return {0, 0, 0};
}