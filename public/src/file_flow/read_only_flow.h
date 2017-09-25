/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   read_only_flow.h
 * Author: chengyeke
 *
 * Created on 2017年8月21日, 上午9:01
 */

#ifndef READ_ONLY_FLOW_H
#define READ_ONLY_FLOW_H
#include "platform.h"

class read_only_flow {
public:
    read_only_flow();
    virtual ~read_only_flow();
    /**
     * 
     * @return Count of the object
     */
    virtual int get_count() = 0;
    
    /**
     * 
     * @return 
     */
    virtual WORD get_comm_phase_no() = 0;
    
    /**
     * 
     * @param id_
     * @param object_
     * @param length_ buffer size
     * @return length of object
     */
    virtual int get(int id_, void *object_, int length_) = 0;
private:

};

#endif /* READ_ONLY_FLOW_H */

