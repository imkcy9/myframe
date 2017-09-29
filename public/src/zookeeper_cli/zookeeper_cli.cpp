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
#include <assert.h>

#define ZK_CREATED_EVENT 1
#define ZK_DELETED_EVENT 2
#define ZK_CHANGED_EVENT 3
void zktest_dump_stat(const struct Stat *stat) {
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat) {
        fprintf(stderr, "null\n");
        return;
    }
    tctime = stat->ctime / 1000;
    tmtime = stat->mtime / 1000;

    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);

    fprintf(stderr, "\tctime = %s\tczxid=%lx\n"
            "\tmtime=%s\tmzxid=%lx\n"
            "\tversion=%x\taversion=%x\n"
            "\tephemeralOwner = %lx\n",
            tctimes, stat->czxid,
            tmtimes, stat->mzxid,
            (unsigned int) stat->version, (unsigned int) stat->aversion, stat->ephemeralOwner);


}

void zktest_stat_completion(int rc, const struct Stat *stat, const void *data) {
    fprintf(stderr, "%s: rc = %d Stat:\n", (char*) data, rc);
    zktest_dump_stat(stat);
}

void zktest_void_completion(int rc, const void *data) {
    fprintf(stderr, "[%s]: rc = %d\n", (char*) (data == 0 ? "null" : data), rc);
}

void zktest_string_completion(int rc, const char *name, const void *data) {
    fprintf(stderr, "[%s]: rc = %d\n", (char*) (data == 0 ? "null" : data), rc);
    if (!rc) {
        fprintf(stderr, "\tname = %s\n", name);
    }
}

void zktest_data_completion(int rc, const char *value, int value_len,
        const struct Stat *stat, const void *data) {
    fprintf(stderr, "[%s]: rc = %d\n", (char*) (data == 0 ? "null" : data), rc);
    if (!rc) {
        fprintf(stderr, "\tname = %s\n", value);
    }
}

void zktest_watcher_g(zhandle_t* zh, int type, int state,
        const char* path, void* watcherCtx) {
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    //printf("watcherCtx: %s\n", (char *)watcherCtx);

    // zhandle_t* handle = (zhandle_t*)watcherCtx;
    assert(watcherCtx);
    zookeeper_cli::event_info_* pevent = (zookeeper_cli::event_info_*)watcherCtx;
    zhandle_t** pzkhandle = pevent->pzkhandle_;
    zhandle_t* handle = *pzkhandle;
    zoo_aexists(handle, path, 1, zktest_stat_completion, "zoo_aexists");
    assert(pevent->event_);
    switch (type) {
        case ZK_CREATED_EVENT:
            pevent->event_->on_zookeeper_create(path);
            break;
        case ZK_DELETED_EVENT:
            pevent->event_->on_zookeeper_delete(path);
            break;
        case ZK_CHANGED_EVENT:
            pevent->event_->on_zookeeper_changed(path);
            break;
        default:
            pevent->event_->on_zookeeper_other(type,state,path);
            break;
    }
    
}

zookeeper_cli::zookeeper_cli()
: zkhandle(0) {
    m_event_info.pzkhandle_ = &zkhandle;
    m_event_info.event_ = 0;
}

zookeeper_cli::~zookeeper_cli() {
    if (zkhandle) {
        zookeeper_close(zkhandle);
    }
}

bool zookeeper_cli::init(const char* hosts, int timeout, zk_event* event, bool readonly) {
    int flag = readonly ? ZOO_READONLY : 0;
    //m_event_info.pzkhandle_ = pzkhandle;
    m_event_info.event_ = event;
    zkhandle = zookeeper_init(hosts,
            zktest_watcher_g, timeout, 0, &m_event_info, flag);
    if (zkhandle == NULL) {
        fprintf(stderr, "Error when connecting to zookeeper servers...\n");
        return false;
    }
    //m_event = event;
    return true;
}

bool zookeeper_cli::acreate_node(std::string path, std::string value, bool infinity) {
    return acreate_node(path.c_str(), value.c_str(), value.size(), infinity);
}

bool zookeeper_cli::acreate_node(const char* path, const char* value, size_t valuelen, bool infinity) {
    int flags = infinity ? 0 : ZOO_EPHEMERAL;
    int ret = -1;
    ret = zoo_acreate(zkhandle, path, value, valuelen,
            &ZOO_OPEN_ACL_UNSAFE, flags /* ZOO_SEQUENCE */,
            zktest_string_completion, "acreate");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_acreate");
        return false;
    }
    return true;
}

bool zookeeper_cli::create_node(std::string path, std::string value, bool infinity) {
    return create_node(path.c_str(), value.c_str(), value.size(), infinity);
}

bool zookeeper_cli::create_node(const char* path, const char* value, size_t valuelen, bool infinity) {
    int flags = infinity ? 0 : ZOO_EPHEMERAL;
    int ret = -1;
    char buffer[1024];
    ret = zoo_create(zkhandle, path, value, valuelen,
            &ZOO_OPEN_ACL_UNSAFE, flags /* ZOO_SEQUENCE */,
            buffer, 1024);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_create");
        return false;
    }
    return true;
}



bool zookeeper_cli::adelete_node(std::string path) {
    return adelete_node(path.c_str());
}

bool zookeeper_cli::adelete_node(const char* path) {
    int ret = zoo_adelete(zkhandle, path, 0, zktest_void_completion, "adelete");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
        return false;
    }
    return true;
}

bool zookeeper_cli::delete_node(std::string path) {
    return delete_node(path.c_str());
}

bool zookeeper_cli::delete_node(const char* path) {
    int ret = zoo_delete(zkhandle, path, 0);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_delete");
        return false;
    }
    return true;
}


bool zookeeper_cli::aexists(const char* path, bool watcher /* = true */) {
    int ret = zoo_aexists(zkhandle, path, watcher, zktest_stat_completion, "aexists");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "aexists");
        return false;
    }
    return true;
}

bool zookeeper_cli::exists(const char* path, bool watcher) {
    Stat sta;
    int ret = zoo_exists(zkhandle, path, watcher, &sta);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_exists");
        return false;
    }
    return true;
}

bool zookeeper_cli::aget(const char* path, bool watcher) {
    int ret = zoo_aget(zkhandle, path, 1, zktest_data_completion, zkhandle);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_aget");
        return false;
    }
    return true;
}

std::string zookeeper_cli::get(const char* path, bool watcher) {
    Stat sta;
    char buffer[1024];
    int size = sizeof(buffer);
    int ret = zoo_get(zkhandle, path, watcher, buffer, &size, &sta);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "zoo_get");
        return "";
    }
    return std::string(buffer,size);
}
