/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sqlconnector.h
 * Author: chengyeke
 *
 * Created on 2017年9月13日, 下午1:31
 */

#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include <vector>
#include <string>
#include "log.h"


class sqlconnector {
public:
    sqlconnector(const sql::SQLString& hostName, const sql::SQLString& dbName, const sql::SQLString& userName, const sql::SQLString& password);

    virtual ~sqlconnector();
    
    virtual const char* get_tablename() = 0;
    
    
    sql::PreparedStatement* get_pre_insert_stmt() {
        if(!pre_insert_stmt)
            status_check();
        return pre_insert_stmt;
    }
    
    sql::PreparedStatement* get_pre_update_stmt() {
        if(!pre_update_stmt)
            status_check();
        return pre_update_stmt;
    }
    
    sql::ResultSet* query(sql::SQLString sql) {
        if(!stmt)
            status_check();
        return stmt->executeQuery(sql);
    }
    
    bool keep_connected() {
        bool connected = true;
        if(con && con->isClosed()) {
            connected = con->reconnect();
            LOG_DEBUG("mysql 重新连接");
        }
        return connected;
    }
protected:
    void add_column_member(const uint32_t index, const sql::SQLString& colMember);
private:
    void status_check();
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::PreparedStatement *pre_insert_stmt;
    sql::PreparedStatement *pre_update_stmt;
    sql::SQLString hostName_;
    sql::SQLString userName_;
    sql::SQLString password_;
    sql::SQLString dbName_;
    std::vector<sql::SQLString> column_members_;
};




#endif /* SQLCONNECTOR_H */

