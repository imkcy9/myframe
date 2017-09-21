/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cache_flow.cpp
 * Author: chengyeke
 * 
 * Created on 2017年8月21日, 上午11:04
 */

#include "cache_flow.h"

cached_flow::cached_flow(bool bSyncFlag, int nMaxObjects, int nDataBlockSize)
: cache_list_(nDataBlockSize) {
    max_objects = nMaxObjects;
    data_block_size = nDataBlockSize;
    under_flow = NULL;
    sync_flag = bSyncFlag;
    comm_phase_no = 0;
    push_count = 0;
    push_size_byte = 0;
    push_sizeM = 0;
    //event_handler = NULL;
    clear();
}

cached_flow::~cached_flow() {
    if (under_flow)
        delete under_flow;
    under_flow = NULL;
}

void cached_flow::clear() {
    first_id = 0;
    node_queue_.clear();
    cache_list_.clear();
}

int cached_flow::attach_under_flow(flow* flow_) {
    under_flow = flow_;
    comm_phase_no = flow_->get_comm_phase_no();

    clear();
    char* buf = new char[data_block_size];
    for (int i = 0; i < under_flow->get_count(); i++) {
        int len = under_flow->get(i, buf, data_block_size);
        append(buf, len);
    }
    delete buf;
    return 0;
}

flow* cached_flow::detach_under_flow() {
    flow* pflow = under_flow;
    under_flow = NULL;
    return pflow;
}

int cached_flow::get_count() {
    return node_queue_.size();
}

const int* cached_flow::get_count_ptr() {
    if (under_flow == NULL) {
        return NULL;
    }
    return under_flow->get_count_ptr();
}

const int* cached_flow::get_push_ptr() {
    return &push_count;
}

const int* cached_flow::get_push_size() {
    return &push_sizeM;
}

int cached_flow::append(void* object_, int length_) {
    int count = node_queue_.size();
    if (max_objects > 0 && count - first_id >= max_objects) {
        if (under_flow != NULL &&
                under_flow->get_count() < first_id) {
            return -1;
        }
        pop_front();
    }

    tcached_flow_node node;
    node.address = cache_list_.push_back(object_, length_);
    node.size = length_;
    node_queue_.push_back(node);
    if (sync_flag) {
        sync_under_flow(count);
    }
    
    push_count = count;
    push_size_byte = push_size_byte + length_ + sizeof(tcached_flow_node);
    push_sizeM = push_size_byte/(1024);
    
    //if(event_handler)
    //  event_handler->notify();
    
    return count;
}

int cached_flow::get(int id_, void* object_, int length_) {
    if(id_ >= first_id) {
        tcached_flow_node node = node_queue_[id_];
        if(node.size > length_) {
            assert(false&&"Insufficient buffer length while reading CFlow");
        }
    }
}

int cached_flow::get_under_count() {
    if(under_flow == NULL) {
        return -1;
    }
    return under_flow->get_count();
}

int cached_flow::sync_under_flow(int id_) {
    int under_count = get_under_count();
    if(id_ != under_count) {
        return -1;
    }
    
    if(get_count() == under_count) {
        return -1;
    }
    
    tcached_flow_node node = node_queue_[under_count];
    under_flow->append(node.address,node.size);
    return 0;
}

WORD cached_flow::get_comm_phase_no() {
    return comm_phase_no;
}

void cached_flow::set_comm_phase_no(WORD comm_phase_no_) {
    if(comm_phase_no != comm_phase_no_) {
        clear();
        comm_phase_no = comm_phase_no_;
    }
    if(under_flow != NULL) {
        under_flow->set_comm_phase_no(comm_phase_no);
    }
}

bool cached_flow::truncate(int count_) {
    if(under_flow) {
        if(under_flow->truncate(count_))
            cached_flow::attach_under_flow(under_flow);
        else
            return false;
    } else {
        if(count_ == 0) {
            cached_flow::clear();
        } else {
            assert(false&&"CCachedFlow can't truncate great 0 ");
            return false;
        }
    }
}

bool cached_flow::pop_front() {
    if(first_id >= get_count()) {
        return false;
    }
    
    cache_list_.pop_front(node_queue_[first_id].size);
    node_queue_[first_id].address = NULL;
    first_id++;
    return true;
}

void cached_flow::attach_observer(/**/) {
    //
}

void cached_flow::detach_observer() {
    // handler = NULL
}








