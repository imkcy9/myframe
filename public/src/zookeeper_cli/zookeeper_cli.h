/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zookeeper.h
 * Author: chengyeke
 *
 * Created on 2017年9月26日, 下午5:11
 */

#ifndef ZOOKEEPER_H
#define ZOOKEEPER_H
#include <zookeeper/zookeeper.h>
#include <string>

struct zk_event {
    virtual ~zk_event() {};
    virtual void on_zookeeper_delete(const char* path){};
    virtual void on_zookeeper_create(const char* path){};
    virtual void on_zookeeper_changed(const char* path){};
    virtual void on_zookeeper_other(int event, int state, const char* path){};
};
class zookeeper_cli {
public:
    zookeeper_cli();
    
    virtual ~zookeeper_cli();
    
    bool init(const char* hosts, int timeout, zk_event* event, bool readonly = true);
    /**
     * 创建结点
     * @param path 结点
     * @param pathlen 结点字符串长度
     * @param infinity 默认false:临时节点   true:永久结点
     * @return 
     */
    bool create_node(const char* path, const char* value, size_t valuelen, bool infinity = false);
    
    bool create_node(std::string path, std::string value, bool infinity = false);
    /**
     * 删除结点
     * @param path
     * @param pathlen
     * @return 
     */
    bool delete_node(const char* path);
    bool delete_node(std::string path);
    
    bool exists(const char* path);
    
    bool get(const char* path);
    
    //bool set_node()
    struct event_info_ {
        zhandle_t** pzkhandle_;
        zk_event* event_;
    }m_event_info;
private:

    
    zhandle_t* zkhandle;// = NULL;
    //zhandle_t** pzkhandle;// = &zkhandle;
    //zk_event* m_event;
};

#endif /* ZOOKEEPER_H */

