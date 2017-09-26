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

class zookeeper_cli {
public:
    zookeeper_cli();
    
    virtual ~zookeeper_cli();
    
    bool init(const char* hosts, int timeout,  bool readonly = true);
    /**
     * 创建结点
     * @param path 结点
     * @param pathlen 结点字符串长度
     * @param infinity 默认false:临时节点   true:永外结点
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
private:

    zhandle_t* zkhandle;// = NULL;
    zhandle_t** pzkhandle;// = &zkhandle;
};

#endif /* ZOOKEEPER_H */

