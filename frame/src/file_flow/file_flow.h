/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   file_flow.h
 * Author: chengyeke
 *
 * Created on 2017年8月21日, 下午4:03
 */

#ifndef FILE_FLOW_H
#define FILE_FLOW_H
#include "platform.h"
#include "flow.h"
#include "zmq.hpp"

class file_flow : public flow {
public:
    file_flow(const char* file_name_, const char* file_path_, bool reuse_);
    file_flow(int flow_id_, const char* file_path_, bool reuse_);
    
    virtual int get_count();

    virtual int append(void* object_, int length_);

    virtual WORD get_comm_phase_no();

    virtual void set_comm_phase_no(WORD comm_phase_no_);

    virtual bool truncate(int count_);

    virtual int get(int id_, void* object_, int length_);
    
    virtual int get(int id_, zmq::message_t* msg_);

    virtual const int* get_count_ptr();

    virtual ~file_flow();
protected:
    bool init_file();
    
    fpos_t get_content_size();
    
    void open_file(const char* flow_name_, const char* path_, bool reuse_);
    
    void close_file();
    
    fpos_t get_offset(int id_);
    
    bool truncated_file();


private:
    FILE* fp_id_file;
    FILE* fp_content_file;
    vector<fpos_t> block_offset;
    int count;
    fpos_t content_size;
    int last_read_id;
    fpos_t last_read_offset;
    WORD comm_phase_no;
    
    CRITICAL_VAR critical_var;
};

#endif /* FILE_FLOW_H */

