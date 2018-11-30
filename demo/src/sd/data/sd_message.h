/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sd_message.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午4:50
 */

#ifndef SD_MESSAGE_H
#define SD_MESSAGE_H
#include "field_struct.h"
namespace kt {
    class sd_message_t : public field_struct_t {
    public:
        sd_message_t(int message_tag, int service_tag, int operation_id)
        :field_struct_t(true)
        ,_message_tag(message_tag)
        ,_service_tag(service_tag)
        ,_operation_id(operation_id)
        {

        }
        
        sd_message_t(int message_tag, int service_tag, int operation_id, field_struct_t& field_struct)
        :field_struct_t(field_struct.get_fields())
        ,_message_tag(message_tag)
        ,_service_tag(service_tag)
        ,_operation_id(operation_id)
        {

        }
        
        int GetMessage_tag() const {
            return _message_tag;
        }

        int GetOperation_id() const {
            return _operation_id;
        }

        int GetService_tag() const {
            return _service_tag;
        }

        
    private:
        int _message_tag;
        int _service_tag;
        int _operation_id;
    };
}


#endif /* SD_MESSAGE_H */

