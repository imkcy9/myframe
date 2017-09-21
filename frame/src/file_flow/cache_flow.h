/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cache_flow.h
 * Author: chengyeke
 *
 * Created on 2017年8月21日, 上午11:04
 */

#ifndef CACHE_FLOW_H
#define CACHE_FLOW_H
#include "flow.h"
#include "cache_list.h"

struct tcached_flow_node {
    void* address;
    int size;
};

#define MAX_NODE  0x7fffffff
#define BLOCK_SIZE  0x00010000
#define BUCKET_SIZE (MAX_NODE/BLOCK_SIZE)

class cached_flow_node_vector {
public:

    cached_flow_node_vector() {
        node_count = 0;
        memset(buckets, 0, BUCKET_SIZE * sizeof (tcached_flow_node*));
    }

    ~cached_flow_node_vector() {
        clear();
    }

    inline tcached_flow_node & operator[](size_t n) {
        return buckets[n / BLOCK_SIZE][n & (BLOCK_SIZE - 1)];
    }

    inline void push_back(tcached_flow_node& node) {
        if (node_count % BLOCK_SIZE == 0) {
            buckets[node_count / BLOCK_SIZE] = new tcached_flow_node[BLOCK_SIZE];
        }
        (*this)[node_count] = node;
        node_count++;
    }

    inline size_t size() {
        return node_count;
    }

    inline void clear() {
        node_count = 0;
        for (int i = 0; i < BUCKET_SIZE; i++) {
            if (buckets[i] == NULL) {
                break;
            }
            delete buckets[i];
        }
        memset(buckets, 0, BUCKET_SIZE * sizeof (tcached_flow_node*));
    }

private:
    volatile int node_count;
    tcached_flow_node* buckets[BUCKET_SIZE];
};

/**
 * 
 */
class cached_flow : public flow {
public:
    cached_flow(bool bSyncFlag, int nMaxObjects, int nDataBlockSize);

    virtual ~cached_flow();

    virtual void set_comm_phase_no(WORD comm_phase_no_);

    virtual bool truncate(int count_);

    virtual int append(void *object_, int length_);

    virtual const int *get_count_ptr();
    
    virtual const int *get_push_ptr();
    
    virtual const int *get_push_size();

    virtual int get_count();

    virtual WORD get_comm_phase_no();
    
    virtual int get(int id_, void *object_, int length_);
    
    virtual int attach_under_flow(flow* flow_);
    
    virtual flow* detach_under_flow();
    
    int get_under_count();
    
    void clear();
    
    int sync_under_flow(int id_);
    
    //virtual bool truncate(int count_);
    
    //virtual int append(void* object_, int length_);
    
    //virtual int get(int id_, void* object_, int length_);
    
    bool pop_front();
    
    virtual void attach_observer(/*pipe_event_handler*/);
    
    virtual void detach_observer();

private:
    
    bool sync_flag;
    flow* under_flow;
    
    volatile int first_id;
    
    int max_objects;
    int data_block_size;
    
    cache_list cache_list_;
    
    typedef cached_flow_node_vector node_queue;
    node_queue node_queue_;
    
    WORD comm_phase_no;
    
    int push_count;
    size_t push_size_byte;
    int push_sizeM;
    
    //pipe_even_handler* event_handler;
};

#endif /* CACHE_FLOW_H */

