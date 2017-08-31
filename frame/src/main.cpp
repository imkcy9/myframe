/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: chengyeke
 *
 * Created on 2017年8月25日, 上午10:53
 */

#include <cstdlib>
#include <signal.h>
#include "config.h"
#include "spdlog/spdlog.h"
#include "log.h"
#include "zmq_poller_reactor.h"
using namespace std;

/*
 * 
 */
void prog_exit(int signo);
void init_main(int argc, char **argv, bool& isdaemon);
char g_configfilename[128];
int main(int argc, char** argv) {
    bool isdaemon = false;
    init_main(argc,argv,isdaemon);
    
    //init configure
    bool ret = config::Instance()->load_file(g_configfilename);
    if(!ret) {
        return -1;
    }
    //LOG_INFO("hello 3333 {}","777");
    //init logger
    LOGGER::init(isdaemon,config::Instance()->get_basic_logPriority(),config::Instance()->get_basic_logfile());
    
    
    zmq::context_t ctx;
    zmq_poller_reactor reactor(&ctx);
    
    zmq::socket_t signal(ctx,ZMQ_PUB);
    signal.bind("inproc://signal");

    reactor.start();
    
    while(true) {
        signal.send("killed",7);
        zmq_sleep(1);
        LOG_INFO("send kill");
    }
    
    reactor.join();
    
    XMLDocument doc;
    XMLError err = doc.LoadFile( "../conf/cfg.xml" );
    const char* str = doc.FirstChildElement("config")->FirstChildElement("basic")->Attribute("logfile");
    LOG_INFO("hello {}",str);
    LOG_DEBUG("hello {}",str);
    LOG_WARN("hello {}",str);
    LOG_ERROR("hello {}",str);
    LOG_CRITICAL("hello {}",str);
    LOG_INFO("hello {}",str);
    return 0;
}

void init_main(int argc, char **argv, bool& isdaemon) {
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
    signal(SIGINT, prog_exit);
    signal(SIGKILL, prog_exit);
    signal(SIGTERM, prog_exit);
}

void prog_exit(int /*signo*/) {
    //signal(SIGINT, SIG_IGN);
    //signal(SIGKILL, SIG_IGN);
    //signal(SIGTERM, SIG_IGN);
}
