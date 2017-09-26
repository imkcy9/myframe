/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zookeeper.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月26日, 下午5:11
 */

#include "zookeeper_cli.h"
#include <zookeeper/zookeeper_log.h>

void zktest_dump_stat(const struct Stat *stat)
{
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat) {
        fprintf(stderr,"null\n");
        return;
    }
    tctime = stat->ctime/1000;
    tmtime = stat->mtime/1000;
       
    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);
       
    fprintf(stderr, "\tctime = %s\tczxid=%lx\n"
    "\tmtime=%s\tmzxid=%lx\n"
    "\tversion=%x\taversion=%x\n"
    "\tephemeralOwner = %lx\n",
     tctimes, stat->czxid,
     tmtimes, stat->mzxid,
    (unsigned int)stat->version, (unsigned int)stat->aversion, stat->ephemeralOwner);
    
    
}

void zktest_stat_completion(int rc, const struct Stat *stat, const void *data)
{
    fprintf(stderr, "%s: rc = %d Stat:\n", (char*)data, rc);
    zktest_dump_stat(stat);
}

void zktest_void_completion(int rc, const void *data)
{
    fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
}

void zktest_string_completion(int rc, const char *name, const void *data)
{
    fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
    if (!rc) {
        fprintf(stderr, "\tname = %s\n", name);
    }
}

void zktest_data_completion(int rc, const char *value, int value_len,
        const struct Stat *stat, const void *data) {
    fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
    if (!rc) {
        fprintf(stderr, "\tname = %s\n", value);
    }
}

void zktest_watcher_g(zhandle_t* zh, int type, int state,
        const char* path, void* watcherCtx)
{
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    //printf("watcherCtx: %s\n", (char *)watcherCtx);
    
   // zhandle_t* handle = (zhandle_t*)watcherCtx;
    zhandle_t** pzkhandle = (zhandle_t**)watcherCtx;
    zhandle_t* handle = *pzkhandle;
    zoo_aexists(handle, path, 1, zktest_stat_completion, "zoo_aexists");
}


zookeeper_cli::zookeeper_cli()
:zkhandle(0)
,pzkhandle(&zkhandle) {
}

zookeeper_cli::~zookeeper_cli() {
    if(zkhandle) {
        zookeeper_close(zkhandle);
    }
}

bool zookeeper_cli::init(const char* hosts, int timeout, bool readonly) {
    int flag = readonly? ZOO_READONLY : 0;
    zkhandle = zookeeper_init(hosts,
            zktest_watcher_g, timeout, 0, pzkhandle, flag);
    if (zkhandle == NULL) {
        fprintf(stderr, "Error when connecting to zookeeper servers...\n");
        return false;
    }
    return true;
}

bool zookeeper_cli::create_node(std::string path, std::string value, bool infinity) {
    return create_node(path.c_str(),value.c_str(), value.size(),infinity);
}

bool zookeeper_cli::create_node(const char* path, const char* value, size_t valuelen, bool infinity) {
    int flags = infinity? 0 : ZOO_EPHEMERAL;
    int ret = -1;
    ret = zoo_acreate(zkhandle, path, value, valuelen,
            &ZOO_OPEN_ACL_UNSAFE, flags /* ZOO_SEQUENCE */,
            zktest_string_completion, "acreate");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "acreate");
        return false;
    }
    return true;
}


bool zookeeper_cli::delete_node(std::string path) {
    return delete_node(path.c_str());
}

bool zookeeper_cli::delete_node(const char* path) {
    int ret = zoo_adelete(zkhandle, path, 0, zktest_void_completion, "adelete");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
        return false;
    }
    return true;
}

bool zookeeper_cli::exists(const char* path) {
    int ret = zoo_aexists(zkhandle, path, 1, zktest_stat_completion, "aexists");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "aexists");
        return false;
    }
    return true;
}

bool zookeeper_cli::get(const char* path) {
    int ret = zoo_aget(zkhandle, path, 1, zktest_data_completion, zkhandle);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "aget");
        return false;
    }
    return true;
}
