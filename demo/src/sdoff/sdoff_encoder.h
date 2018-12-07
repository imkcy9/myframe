/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_encoder.h
 * Author: kecy
 *
 * Created on 2018年11月23日, 下午5:27
 */

#ifndef SDOFF_ENCODER_H
#define SDOFF_ENCODER_H
#include <string>
#include "../sd/data/field_struct.h"
#include "./util/dumptool.h"
namespace kt {
    class sd_message_t;

    class sdoff_encoder {
    public:
        sdoff_encoder();
        sdoff_encoder(const sdoff_encoder& orig);
        virtual ~sdoff_encoder();

        std::string& encode(kt::sd_message_t* sd_message_t);
    private:
        void encode_field_list(std::string& buffer, kt::field_struct_t& field_list);
        void encode_field_value(std::string& buffer, kt::field_value_t& value);
        void encode_struct(std::string& buffer, kt::field_struct_t& struct_field_list);
        void encode_array(std::string& buffer, kt::field_value_t& value);
        void encode_primitive_type(std::string& buffer, kt::field_value_t& value, kt::field_enum_type_t type);
        void encode_signed_long(std::string& buffer, long value);
        void encode_unsigned_long(std::string& buffer, long value, long mask);
        void encode_string(std::string& buffer, std::string value);
        std::string _in_progress;
        const long STOP_BIT = 0x80;
        const char BIT_MASK = 0x7F;
        const char BYTE = 'b';
        const char INT32 = 'i';
        const char UINT32 = 'I';
        const char UINT64 = 'L';
        const char INT64 = 'l';
        const char STRING = 's';
        const char STRUCT = 'S';
        const char ARRAY = 'A';
        const char REAL32 = 'r';
        const char REAL64 = 'R';
        const char INT16 = 'w';
        const char UINT16 = 'W';
        const char TRUE = 't';
        const char FALSE = 'f';
        const char OCTETS = 'o';
    };
}

#endif /* SDOFF_ENCODER_H */

