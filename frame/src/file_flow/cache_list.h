/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cache_list.h
 * Author: chengyeke
 *
 * Created on 2017年8月21日, 上午9:45
 */

#ifndef CACHE_LIST_H
#define CACHE_LIST_H
#include <cstddef>
class cache_node;
class cache_list {
public:
    cache_list(int data_block_size_);
    
    virtual ~cache_list();
    
    void* get_data(int &len_);
    
    int pop_front(int len);
    
    void* push_back(void* data_, int datalen_);
    
    bool is_empty();
    
    void clear();
    
private:

    cache_list(const cache_list& orig);
    
    cache_node* head_node;
    cache_node* tail_node;
    int data_block_size;
};

#endif /* CACHE_LIST_H */

