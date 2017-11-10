/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: chengyeke
 *
 * Created on 2017年9月12日, 下午12:07
 */

#include <cstdlib>
#include "application.h"
#include "update_thread.h"
using namespace std;

/*
 * 
 */
void prog_exit(int signo);

application<update_thread> app;
int main(int argc, char** argv) {

    bool ret = app.init_main(argc,argv,prog_exit);
    if(ret) {
        app.start();
        app.join();
    }
    return 0;
}

void prog_exit(int /*signo*/) {
    LOG_WARN("exit");
    app.stop();
}
