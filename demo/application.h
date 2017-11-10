/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   application.h
 * Author: chengyeke
 *
 * Created on 2017年9月12日, 上午11:35
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "log.h"
#include "config.h"
#include "thread.h"
#include "update_thread.h"

template<class PROCESS>
class application : public thread_t {
public:
    application();
    virtual ~application();
    
    bool init_main(int argc, char** argv, void(sig_fun)(int));
    
    virtual void run();
    virtual bool before_start();
    virtual void before_end();
    
    void stop();
private:

    PROCESS* m_process;
    
    bool init_config();
    char g_configfilename[128];
    bool isdaemon = false;
    zmq::context_t* m_ctx;
};

template<class PROCESS>
application<PROCESS>::application() 
:m_process(0)
,isdaemon(false)
,m_ctx(0){

}

template<class PROCESS>
application<PROCESS>::~application() {
    if(m_ctx) {
        m_ctx->close();
	delete m_ctx;
    }
}

template<class PROCESS>
bool application<PROCESS>::before_start() {
    if(!m_process) {
        m_process = new update_thread(m_ctx);
    }
    if(!m_process->init())
        return false;

    return true;
}

template<class PROCESS>
void application<PROCESS>::run() {
    m_process->start();
}

template<class PROCESS>
void application<PROCESS>::before_end() {
    m_process->join();
    LOG_INFO("application end");
    LOGGER::dropall();
}

template<class PROCESS>
bool application<PROCESS>::init_main(int argc, char** argv, void(sig_fun)(int)) {

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

template<class PROCESS>
bool application<PROCESS>::init_config() {
    char buffer[100];
    getcwd(buffer, sizeof(buffer));
    printf("The current directory is: %s\n", buffer);
    
    //init configure
    bool ret = config::Instance()->load_file(g_configfilename);
    if(!ret) {
        return false;
    }

#ifdef USE_GLOG
    ret = LOGGER::init(argv[0], isdaemon,config::Instance()->get_basic_logPriority(),config::Instance()->get_basic_logfile());
#else
    ret = LOGGER::init(config::Instance()->get_basic_logprefix(),
            isdaemon,config::Instance()->get_basic_logPriority(),
            config::Instance()->get_basic_logfile());
#endif
    LOG_INFO("app init success..");
    return ret;
}

template<class PROCESS>
void application<PROCESS>::stop() {
    if(m_process) {
        m_process->stop();
    }
}

#endif /* APPLICATION_H */

