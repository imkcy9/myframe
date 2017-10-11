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

using namespace std;
using namespace sql;


application::application() 
:m_update_thread(0)
,isdaemon(false)
,m_ctx(4){

}

application::~application() {
    m_ctx.close();
}

bool application::before_start() {
    if(!m_update_thread) {
        m_update_thread = new update_thread(&m_ctx);
    }
    if(!m_update_thread->init())
        return false;

    return true;
}

void application::run() {
    m_update_thread->start();
}

void application::before_end() {
    m_update_thread->join();
    LOG_INFO("application end");
    LOGGER::dropall();
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
    if(m_update_thread) {
        m_update_thread->stop();
    }
}
