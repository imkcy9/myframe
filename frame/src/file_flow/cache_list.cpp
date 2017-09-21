/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cache_list.cpp
 * Author: chengyeke
 * 
 * Created on 2017年8月21日, 上午9:45
 */

#include "cache_list.h"
#include <string.h>

class cache_node {
public:

    cache_node(int bufsize_) {
        bufsize = bufsize_;
        buffer = new char[bufsize];
        length = 0;
        data = buffer;
        next = NULL;
    }

    ~cache_node() {
        delete buffer;
    }

    void* get_data(int& len_) {
        if(length == 0) {
            return NULL;
        }
        len_ = (length < len_) ?length:len_;
        return data;
    }
    
    int pop_front(int len_) {
        if(len_ > length)
            len_ = length;
        data += len_;
        length -= len_;
        return len_;
    }
    
    void* push_back(void* data_, int datalen_) {
        char* target = data + length;
        if(target + datalen_ > buffer + bufsize) {
            if(length == 0) {
                data = buffer;
                target = data;
            } else {
                return NULL;
            }
        }
        memcpy(target,data_,datalen_);
        length += datalen_;
        return target;
    }
    
    void set_next(cache_node* pnext_) {
        next = pnext_;
    }
    
    cache_node* get_next() {
        return next;
    }
    
    int get_length() {
        return length;
    }

private:
    int bufsize;
    char* buffer;
    int length;
    char* data;
    cache_node* next;
};

cache_list::cache_list(int data_block_size_) {
    data_block_size = data_block_size_;
    head_node = new cache_node(data_block_size);
    tail_node = head_node;
}

cache_list::cache_list(const cache_list& orig) {
}

cache_list::~cache_list() {
    cache_node* pnode = head_node;
    cache_node* pnext = NULL;
    while(pnode != NULL) {
        pnext = pnode->get_next();
        delete pnode;
        pnode = pnext;
    }
}

void* cache_list::push_back(void* data_, int datalen_) {
    void* target = tail_node->push_back(data_,datalen_);
    if(target != NULL) {
        return target;
    }
    cache_node* pnode = new cache_node(data_block_size);
    tail_node->set_next(pnode);
    tail_node = pnode;
    
    target = tail_node->push_back(data_,datalen_);
    return target;
}

int cache_list::pop_front(int len) {
    int poplen = head_node->pop_front(len);
    if(head_node->get_length() <= 0) {
        cache_node* pnext = head_node->get_next();
        if(pnext != NULL) {
            head_node->set_next(NULL);
            delete head_node;
            head_node = pnext;
        } else {
            tail_node = head_node;
        }
    }
    return poplen;
}

bool cache_list::is_empty() {
    int len = 1;
    return (get_data(len) == NULL);
}

void cache_list::clear() {
    while(head_node->get_length() > 0) {
        pop_front(head_node->get_length());
    }
}




void* cache_list::get_data(int& len_) {
    return head_node->get_data(len_);
}
