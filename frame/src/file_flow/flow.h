/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   flow.h
 * Author: chengyeke
 *
 * Created on 2017年8月21日, 上午8:54
 */

#ifndef FLOW_H
#define FLOW_H
#include "read_only_flow.h"

class flow : public read_only_flow {
public:
    flow();
    
    virtual ~flow();
    
    virtual void set_comm_phase_no(WORD comm_phase_no_) = 0;
    
    virtual bool truncate(int count_) = 0;
    
    virtual int append(void *object_, int length_) = 0;
    
    virtual const int *get_count_ptr() = 0;
    
private:
    
};

#endif /* FLOW_H */

