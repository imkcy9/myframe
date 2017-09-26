/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "sqlconnector.h"

#include <sstream>
#include <stdint.h>


sqlconnector::sqlconnector(const sql::SQLString& hostName,const sql::SQLString& dbName, const sql::SQLString& userName, const sql::SQLString& password)
: driver(0)
, con(0)
, stmt(0)
, pre_insert_stmt(0)
, pre_update_stmt(0){
    hostName_ = hostName;
    userName_ = userName;
    password_ = password;
    dbName_   = dbName;
}


sqlconnector::~sqlconnector() {
}


void sqlconnector::add_column_member(const uint32_t index, const sql::SQLString& colMember) {
    column_members_.push_back(colMember);
}

bool sqlconnector::keep_connected() {
    bool connected = true;
    if (con) {
        if (con->isClosed() || !con->isValid()) {
            connected = con->reconnect();
            bool myTrue = true;
            con->setClientOption("OPT_RECONNECT",&myTrue);
            con->setSchema(dbName_);
            con->setAutoCommit(true);
            LOG_DEBUG("mysql 重新连接");
        }
    }

    return connected;
}


void sqlconnector::status_check() {
    if(!driver) {
        driver = get_driver_instance();
    }
    if(!con && driver) {
        LOG_INFO("mysql hostname:{}, username:{}, password:{}",hostName_.c_str(),userName_.c_str(),password_.c_str());
        con = driver->connect(hostName_,userName_,password_);
        //con = driver->connect("tcp://192.168.19.192:3306","tradepro","trade123456ing");
        bool myTrue = true;
        con->setClientOption("OPT_RECONNECT",&myTrue);
        con->setSchema(dbName_);
        
        con->setAutoCommit(true);
        
    }
    if(con && con->isClosed()) {
        con->reconnect();
    }
    if(!stmt) {
        stmt = con->createStatement();
    }
    if(!pre_insert_stmt) {
        std::ostringstream oss;
        std::string keys;
        std::string values;
        for(size_t inx = 0; inx != column_members_.size(); inx++) {
            keys += column_members_[inx];
            values += "?";
            if(inx != column_members_.size() - 1) {
                keys += ",";
                values += ",";
            }
        }
        oss << "INSERT INTO " 
                << get_tablename()
                << "(" << keys << ")"
                << " VALUES(" << values << ");";
        LOG_TRACE("{}",oss.str());
        pre_insert_stmt = con->prepareStatement(oss.str());
    }
    if(!pre_update_stmt) {
        std::ostringstream oss;
        std::string keys;
        oss << "UPDATE " << get_tablename() << " set ";
        for(size_t inx = 0; inx != column_members_.size(); inx++) {
            oss << column_members_[inx] << "=?";
            if(inx != column_members_.size() - 1) {
                oss << ",";
            }
        }
        oss << " WHERE ID=?";
        LOG_TRACE("{}",oss.str());
        pre_update_stmt = con->prepareStatement(oss.str());
    }
}
