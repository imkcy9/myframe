/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   application.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月12日, 上午11:35
 */

#include "application.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include <iostream>
#include "stcode_updator.h"
using namespace std;
using namespace sql;


application::application() 
:m_pEngine(0)
,m_update_thread(0)
,m_flow_worker(0)
,isdaemon(false)
,m_ctx(0){

}

application::~application() {
    
    if(m_pEngine)
        delete m_pEngine;
    if(m_update_thread)
        delete m_update_thread;
    if(m_flow_worker)
        delete m_flow_worker;
    if(m_ctx) {
        m_ctx->close();
        delete m_ctx;
    }
}

bool application::before_start() {
    if(!m_ctx) {
        m_ctx = new zmq::context_t(4);
    }
    
    if(!m_pEngine) {
        m_pEngine = new md_engine(m_ctx);
    }
    if(!m_update_thread) {
        m_update_thread = new update_thread(m_ctx,m_pEngine);
    }
    if(!m_flow_worker) {
        m_flow_worker = new flow_worker(m_ctx);
    }
    
    if(!m_pEngine->init()) {
        return false;
    }
    if(!m_update_thread->init()) {
        return false;
    }
    
    if(!config::Instance()->get_mdconfig_is_storage()) {
        LOG_WARN("storage mode is off");
    } else if(!m_flow_worker->init()) {
        LOG_ERROR("flow_worker init fail");
        return false;
    }
    return true;
}

void application::run() {
    m_pEngine->start();
    m_update_thread->start();
    m_flow_worker->start();
}

void application::before_end() {
    m_pEngine->join();
    m_update_thread->join();
    m_flow_worker->join();
    LOG_INFO("application end");
    LOGGER::dropall();
}
/* 获取数据库信息 */  
static void GetDBMetaData(Connection *dbcon)   
{  
    if (dbcon->isClosed())   
    {  
        throw runtime_error("DatabaseMetaData FAILURE - database connection closed");  
    }  
  
    cout << "/nDatabase Metadata" << endl;  
    cout << "-----------------" << endl;  
  
    cout << boolalpha;  
  
    /* The following commented statement won't work with Connector/C++ 1.0.5 and later */  
    //auto_ptr < DatabaseMetaData > dbcon_meta (dbcon->getMetaData());  
  
    DatabaseMetaData *dbcon_meta = dbcon->getMetaData();  
  
    cout << "Database Product Name: " << dbcon_meta->getDatabaseProductName() << endl;  
    cout << "Database Product Version: " << dbcon_meta->getDatabaseProductVersion() << endl;  
    cout << "Database User Name: " << dbcon_meta->getUserName() << endl << endl;  
  
    cout << "Driver name: " << dbcon_meta->getDriverName() << endl;  
    cout << "Driver version: " << dbcon_meta->getDriverVersion() << endl << endl;  
  
    cout << "Database in Read-Only Mode?: " << dbcon_meta->isReadOnly() << endl;  
    cout << "Supports Transactions?: " << dbcon_meta->supportsTransactions() << endl;  
    cout << "Supports DML Transactions only?: " << dbcon_meta->supportsDataManipulationTransactionsOnly() << endl;  
    cout << "Supports Batch Updates?: " << dbcon_meta->supportsBatchUpdates() << endl;  
    cout << "Supports Outer Joins?: " << dbcon_meta->supportsOuterJoins() << endl;  
    cout << "Supports Multiple Transactions?: " << dbcon_meta->supportsMultipleTransactions() << endl;  
    cout << "Supports Named Parameters?: " << dbcon_meta->supportsNamedParameters() << endl;  
    cout << "Supports Statement Pooling?: " << dbcon_meta->supportsStatementPooling() << endl;  
    cout << "Supports Stored Procedures?: " << dbcon_meta->supportsStoredProcedures() << endl;  
    cout << "Supports Union?: " << dbcon_meta->supportsUnion() << endl << endl;  
  
    cout << "Maximum Connections: " << dbcon_meta->getMaxConnections() << endl;  
    cout << "Maximum Columns per Table: " << dbcon_meta->getMaxColumnsInTable() << endl;  
    cout << "Maximum Columns per Index: " << dbcon_meta->getMaxColumnsInIndex() << endl;  
    cout << "Maximum Row Size per Table: " << dbcon_meta->getMaxRowSize() << " bytes" << endl;  
  
    cout << "/nDatabase schemas: " << endl;  
  
    auto_ptr < ResultSet > rs ( dbcon_meta->getSchemas());  
  
    cout << "/nTotal number of schemas = " << rs->rowsCount() << endl;  
    cout << endl;  
  
    int row = 1;  
  
    while (rs->next()) {  
        cout << "/t" << row << ". " << rs->getString("TABLE_SCHEM") << endl;  
        ++row;  
    } // while  
  
    cout << endl << endl;  
}  
/* 获取结果集信息 */  
static void GetResultDataMetaBata(ResultSet *rs)   
{  
  
    if (rs -> rowsCount() == 0)   
    {  
        throw runtime_error("ResultSetMetaData FAILURE - no records in the result set");  
    }  
  
    cout << "ResultSet Metadata" << endl;  
    cout << "------------------" << endl;  
  
    /* The following commented statement won't work with Connector/C++ 1.0.5 and later */  
    //auto_ptr < ResultSetMetaData > res_meta ( rs -> getMetaData() );  
  
    ResultSetMetaData *res_meta = rs -> getMetaData();  
  
    int numcols = res_meta -> getColumnCount();  
    cout << "/nNumber of columns in the result set = " << numcols << endl << endl;  
  
    cout.width(20);  
    cout << "Column Name/Label";  
    cout.width(20);  
    cout << "Column Type";  
    cout.width(20);  
    cout << "Column Size" << endl;  
  
    for (int i = 0; i < numcols; ++i)   
    {  
        cout.width(20);  
        cout << res_meta -> getColumnLabel (i+1);  
        cout.width(20);   
        cout << res_meta -> getColumnTypeName (i+1);  
        cout << res_meta -> getColumnType (i+1);  
        cout.width(20);   
        cout << res_meta -> getColumnDisplaySize (i+1) << endl << endl;  
    }  
  
    cout << "/nColumn /"" "<< res_meta -> getColumnLabel(1);  
    cout << "/ belongs to the Table: " << res_meta -> getTableName(1);  
    cout << " which belongs to the Schema:"  << res_meta -> getSchemaName(1) << "\"" << endl << endl;  
}  
/* 打印结果集中的数据 */  
static void RetrieveDataAndPrint(ResultSet *rs, int type, int colidx, string colname)   
{  
  
    /* retrieve the row count in the result set */  
    cout << "/nRetrieved " << rs->rowsCount() << " row(s)." << endl;  
  
    cout << "/nCityName" << endl;  
    cout << "--------" << endl;  
  
    /* fetch the data : retrieve all the rows in the result set */  
    while (rs->next())   
    {  
        if (type == 100)   
        {  
            cout << rs -> getString(colidx) << endl;  
        } else if (type == 200)   
        {  
            cout << rs -> getString(colname) << endl;  
        } // if-else  
    } // while  
  
    cout << endl;  
}  
void application::test_sqlconnector() {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::PreparedStatement *pre_stmt;
    sql::PreparedStatement *pre_stmt2;
    sql::Savepoint *savept;
    sql::ResultSet *res;
    try{
    driver = get_driver_instance();
    con = driver->connect(config::Instance()->get_mysqlconn_dbip(),
            config::Instance()->get_mysqlconn_username(),
            config::Instance()->get_mysqlconn_passwd()
            );
    con->setAutoCommit(false);
    
    con->setSchema(config::Instance()->get_mysqlconn_dbname());
    
    GetDBMetaData(con);
    
    
    
    stmt = con->createStatement();
    
    res = stmt->executeQuery("select * from ts_house");
    
    GetResultDataMetaBata(res);
    
    RetrieveDataAndPrint(res,200,1,"tradingday");
    
    pre_stmt = con->prepareStatement("insert into ts_house(tradingday,houseid,id) values(?,?,?)");
    con->prepareStatement("insert into ts_house(tradingday) values(?)");
    pre_stmt->setString(1,"3456");
    pre_stmt->setInt(2,90);
    pre_stmt->setString(3,"68");
    int updatecount = pre_stmt->executeUpdate();
    LOG_INFO("updatecount = {}",updatecount);
    con->commit();
    pre_stmt->setString(1,"星其日");
    pre_stmt->setInt(2,1845);
    pre_stmt->setString(3,"218");
     updatecount = pre_stmt->executeUpdate();
    LOG_INFO("updatecount = {}",updatecount);
    con->commit();
    
    } catch (sql::SQLException &e) {
        LOG_ERROR("Error {},{},{},{}", e.what(),e.getErrorCode(),e.getSQLState(),e.getSQLStateCStr());
    } catch (std::runtime_error &e) {
        
    }
}


bool application::init_main(int argc, char** argv, void(sig_fun)(int)) {

    memset(g_configfilename, 0, sizeof (g_configfilename));
    int ch;
    while ((ch = getopt(argc, argv, "f:d")) != -1) {
        switch (ch) {
            case 'f':
                strncpy(g_configfilename, optarg, sizeof (g_configfilename));
                break;
            case 'd':
                isdaemon = true;
                break;
        }
    }
    if (strlen(g_configfilename) == 0)
        strcpy(g_configfilename, "../conf/cfg.xml");
    //AC_INFO("configfilename = %s\n", g_configfilename);
    if (isdaemon) {
        int pid;
        signal(SIGCHLD, SIG_IGN);
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(-1);
        }
        else if (pid > 0)
            exit(0);
        setsid();
        int fd;
        fd = open("/dev/null", O_RDWR, 0);
        if (fd != -1) {
            dup2(fd, STDIN_FILENO);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            if (fd > 2)
                close(fd);
        }
    }
    //设置信号处理
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_fun);
    signal(SIGKILL, sig_fun);
    signal(SIGTERM, sig_fun);
    
    return init_config();
}

bool application::init_config() {
    char buffer[100];
    getcwd(buffer, sizeof(buffer));
    printf("The current directory is: %s\n", buffer);
    
    //init configure
    bool ret = config::Instance()->load_file(g_configfilename);
    if(!ret) {
        return false;
    }

    LOGGER::init(isdaemon,config::Instance()->get_basic_logPriority(),config::Instance()->get_basic_logfile());
    LOG_INFO("app init success..");
    return true;
}

void application::stop() {
    if(m_pEngine) {
        m_pEngine->stop();
    }
    if(m_update_thread) {
        m_update_thread->stop();
    }
    if(m_flow_worker) {
        m_flow_worker->stop();
    }
}
