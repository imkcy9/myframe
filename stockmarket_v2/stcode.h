/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stcode.h
 * Author: chengyeke
 *
 * Created on 2017年9月13日, 下午2:58
 */

#ifndef STCODE_H
#define STCODE_H
#include "sqlconnector.h"
#include "config.h"
#include "vector"
#include <sstream>
using namespace std;

class stcode : public sqlconnector {
public:

    enum STOCK_CODE {
        E_SECURITY_CODE = 0,
        E_SECURITY_TYPE,
        E_SHORT_NAME,
        E_EPS,
        E_EPS2,
        E_EPS3,
        E_NAV,
        E_LAST_FIVE_TRADE,
        E_TOTAL_SHARE,
        E_CIRCULATED_SHARE,
        E_CIRCULATED_SHARE_A,
        E_CIRCULATED_SHARE_B,
        E_PRICE_UNIT,
        E_YEAR_PROFIT,
        E_Q4PROFIT_SUM,
        E_LAUNCH_PRICE,
        E_STATUS,
        E_ROE,
        E_REVENUE_PERSHARE,
        E_REPORT_PROFIT

    };

    stcode() :
    sqlconnector(config::Instance()->get_mysqlconn_dbip(),
    config::Instance()->get_mysqlconn_dbname(),
    config::Instance()->get_mysqlconn_username(),
    config::Instance()->get_mysqlconn_passwd()) {

        add_column_member(1, "id");
        add_column_member(2, "SecurityCode");
        add_column_member(3, "SecurityName");
        add_column_member(4, "SecurityType");
        add_column_member(5, "ShortName");
        add_column_member(6, "EPS");
        add_column_member(7, "EPS2");

        add_column_member(8, "EPS3");
        add_column_member(9, "NAV");
        add_column_member(10, "LastFiveTrade");
        add_column_member(11, "TotalShare");
        add_column_member(12, "CirculatedShare");
        add_column_member(13, "CirculatedShareA");
        add_column_member(14, "CirculatedShareB");
        add_column_member(15, "PriceUnit");
        add_column_member(16, "YearProfit");
        add_column_member(17, "Q4ProfitSum");
        add_column_member(18, "LaunchPrice");
        add_column_member(19, "Status");
        add_column_member(20, "ROE");
        add_column_member(21, "RevenuePerShare");
        add_column_member(22, "ReportProfit");
        add_column_member(23, "DecimalNum");
        add_column_member(24, "rzrq");
        add_column_member(25, "sgt");
        add_column_member(26, "hgt");
        add_column_member(27, "EnableSubMarket");
        add_column_member(28, "DbInsertTime");



    };

    virtual ~stcode() {
    };

    const char* get_tablename() override {
        return "t_stcode_copy";
    }

    sql::PreparedStatement* update(vector<std::string>& vec, sql::ResultSet* res, bool update_insert_time = true) {
        std::string::size_type pos = vec[E_SECURITY_CODE].find('_');
        if (pos != std::string::npos) {
            vec[E_SECURITY_CODE] = vec[E_SECURITY_CODE].substr(0, pos);
        }

        get_pre_update_stmt()->setInt64(1, res->getInt64("ID"));
        get_pre_update_stmt()->setString(2, vec[E_SECURITY_CODE]);
        get_pre_update_stmt()->setString(3, res->getString("SecurityName"));
        get_pre_update_stmt()->setString(4, vec[E_SECURITY_TYPE]);
        get_pre_update_stmt()->setString(5, vec[E_SHORT_NAME]);
        get_pre_update_stmt()->setString(6, vec[E_EPS]);
        get_pre_update_stmt()->setString(7, vec[E_EPS2]);
        get_pre_update_stmt()->setString(8, vec[E_EPS3]);
        get_pre_update_stmt()->setString(9, vec[E_NAV]);
        get_pre_update_stmt()->setString(10, vec[E_LAST_FIVE_TRADE]);
        get_pre_update_stmt()->setString(11, vec[E_TOTAL_SHARE]);
        get_pre_update_stmt()->setString(12, vec[E_CIRCULATED_SHARE]);
        get_pre_update_stmt()->setString(13, vec[E_CIRCULATED_SHARE_A]);
        get_pre_update_stmt()->setString(14, vec[E_CIRCULATED_SHARE_B]);
        get_pre_update_stmt()->setString(15, vec[E_PRICE_UNIT]);
        get_pre_update_stmt()->setString(16, vec[E_YEAR_PROFIT]);
        get_pre_update_stmt()->setString(17, vec[E_Q4PROFIT_SUM]);
        get_pre_update_stmt()->setString(18, vec[E_LAUNCH_PRICE]);
        get_pre_update_stmt()->setString(19, vec[E_STATUS]);
        get_pre_update_stmt()->setString(20, vec[E_ROE]);
        get_pre_update_stmt()->setString(21, vec[E_REVENUE_PERSHARE]);
        get_pre_update_stmt()->setString(22, vec[E_REPORT_PROFIT]);
        get_pre_update_stmt()->setInt(23, res->getInt("DecimalNum"));
        get_pre_update_stmt()->setInt(24, res->getInt("rzrq"));
        get_pre_update_stmt()->setInt(25, res->getInt("sgt"));
        get_pre_update_stmt()->setInt(26, res->getInt("hgt"));
        get_pre_update_stmt()->setInt(27, res->getInt("EnableSubMarket"));
        if (update_insert_time) {
            get_pre_update_stmt()->setNull(28, 0);
        } else {
            get_pre_update_stmt()->setDateTime(28, res->getString("DbInsertTime"));
        }

        get_pre_update_stmt()->setInt64(29, res->getInt64("ID"));

        //get_pre_stmt()->executeUpdate();
        return get_pre_update_stmt();
    }

    sql::PreparedStatement* insert(vector<std::string>& vec) {
        std::string::size_type pos = vec[E_SECURITY_CODE].find('_');
        if (pos != std::string::npos) {
            vec[E_SECURITY_CODE] = vec[E_SECURITY_CODE].substr(0, pos);
        }

        get_pre_insert_stmt()->setNull(1, 0);
        get_pre_insert_stmt()->setString(2, vec[E_SECURITY_CODE]);
        get_pre_insert_stmt()->setNull(3, 0);
        get_pre_insert_stmt()->setString(4, vec[E_SECURITY_TYPE]);
        get_pre_insert_stmt()->setString(5, vec[E_SHORT_NAME]);
        get_pre_insert_stmt()->setString(6, vec[E_EPS]);
        get_pre_insert_stmt()->setString(7, vec[E_EPS2]);
        get_pre_insert_stmt()->setString(8, vec[E_EPS3]);
        get_pre_insert_stmt()->setString(9, vec[E_NAV]);
        get_pre_insert_stmt()->setString(10, vec[E_LAST_FIVE_TRADE]);
        get_pre_insert_stmt()->setString(11, vec[E_TOTAL_SHARE]);
        get_pre_insert_stmt()->setString(12, vec[E_CIRCULATED_SHARE]);
        get_pre_insert_stmt()->setString(13, vec[E_CIRCULATED_SHARE_A]);
        get_pre_insert_stmt()->setString(14, vec[E_CIRCULATED_SHARE_B]);
        get_pre_insert_stmt()->setString(15, vec[E_PRICE_UNIT]);
        get_pre_insert_stmt()->setString(16, vec[E_YEAR_PROFIT]);
        get_pre_insert_stmt()->setString(17, vec[E_Q4PROFIT_SUM]);
        get_pre_insert_stmt()->setString(18, vec[E_LAUNCH_PRICE]);
        get_pre_insert_stmt()->setString(19, vec[E_STATUS]);
        get_pre_insert_stmt()->setString(20, vec[E_ROE]);
        get_pre_insert_stmt()->setString(21, vec[E_REVENUE_PERSHARE]);
        get_pre_insert_stmt()->setString(22, vec[E_REPORT_PROFIT]);
        get_pre_insert_stmt()->setInt(23, 2);
        get_pre_insert_stmt()->setInt(24, -1);
        get_pre_insert_stmt()->setInt(25, -1);
        get_pre_insert_stmt()->setInt(26, -1);
        get_pre_insert_stmt()->setInt(27, 1);
        get_pre_insert_stmt()->setNull(28, 0);

        //get_pre_stmt()->executeUpdate();
        return get_pre_insert_stmt();
    }

    void get_result() {
        sql::ResultSet* res = query("select * from t_stcode_copy where EnableSubMarket != 0 and Status != 0");

        while (res && res->next()) {
            std::string td = res->getString("securityname");

            LOG_ERROR("search result {}", td);
        }
        LOG_ERROR("search result count {}", res->rowsCount());

        delete res;
    }

    sql::ResultSet* get_by_securitycode(std::string& securityCode) {
        //std::string sc = securityCode;
        std::string::size_type pos = securityCode.find('_');
        if (pos != std::string::npos) {
            securityCode = securityCode.substr(0, pos);
        }
        ostringstream oss;
        oss << "SELECT * FROM " << get_tablename() << " WHERE SecurityCode='" << securityCode << "' LIMIT 1";
        return query(oss.str());
    }

    std::string get_security_name_by_securityCode(std::string& securityCode) {
        //std::string sc = securityCode;
        std::string::size_type pos = securityCode.find('_');
        if (pos != std::string::npos) {
            securityCode = securityCode.substr(0, pos);
        }
        
        CurlHelp securityNameGetter;
        ostringstream oss;
        oss << "http://hq.sinajs.cn/list=s_" << securityCode;
        //LOG_INFO("{}",oss.str());
        securityNameGetter.perform_to_string(oss.str().c_str());
        std::string outstring;
        std::string rtnstring;
        while (securityNameGetter.get_next_line(outstring) && outstring != "") {
            std::string::size_type pos_begin = outstring.find_first_of("\"");
            std::string::size_type pos_end = outstring.find_first_of(",");
            if (pos_begin != std::string::npos && pos_end != std::string::npos) {
                rtnstring = outstring.substr(pos_begin + 1, pos_end - pos_begin - 1);
                break;
            }
        }
        //std::cout << rtnstring << std::endl;
        if (rtnstring.empty()) {
            rtnstring = get_security_name_by_securityCode_EX(securityCode);
        }
        return rtnstring;
    }

    std::string get_security_name_by_securityCode_EX(const std::string& securityCode) {
        CurlHelp securityNameGetter;
        ostringstream oss;
        //oss << "http://hq.sinajs.cn/list=s_" << securityCode;
        oss << "http://vip.stock.finance.sina.com.cn/api/jsonp.php/var%20ListingDate=/BasicStockSrv.getStockInfo?PaperCode=" << securityCode;
        securityNameGetter.perform_to_string(oss.str().c_str());
        std::string outstring;
        std::string rtnstring;
        while (securityNameGetter.get_next_line(outstring) && outstring != "") {
            //std::string::size_type pos_begin = outstring.find_first_of("\"");
            //std::string::size_type pos_end = outstring.find_first_of(",");
            std::string::size_type pos_begin = outstring.find_first_of("{");
            std::string::size_type pos_end = outstring.find_last_of("}");
            if (pos_begin != std::string::npos && pos_end != std::string::npos) {
                rtnstring = outstring.substr(pos_begin, pos_end - pos_begin);
                //AC_INFO("%s",rtnstring.c_str());
                size_t pos = rtnstring.find("PaperName");
                rtnstring = rtnstring.substr(pos);
                //AC_INFO("%s",rtnstring.c_str());
                pos = rtnstring.find_first_of("\"");
                rtnstring = rtnstring.substr(pos + 1, rtnstring.find_first_of(",") - pos - 2);

                //AC_INFO("%s",rtnstring.c_str());
                break;
            }
        }
        //std::cout << rtnstring << std::endl;
        return rtnstring;
    }

private:

};

#endif /* STCODE_H */

