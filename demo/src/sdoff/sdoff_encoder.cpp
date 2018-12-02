/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sdoff_encoder.cpp
 * Author: kecy
 * 
 * Created on 2018年11月23日, 下午5:27
 */

#include <netinet/in.h>

#include "sdoff_encoder.h"
#include "sd/data/sd_message.h"

kt::sdoff_encoder::sdoff_encoder() {
}

kt::sdoff_encoder::sdoff_encoder(const sdoff_encoder& orig) {
}

kt::sdoff_encoder::~sdoff_encoder() {
}

std::string& kt::sdoff_encoder::encode(kt::sd_message_t* sd_message_) {
    _in_progress.clear();
    
    std::string data;
    encode_unsigned_long(data, sd_message_->GetMessage_tag(), -1);
    encode_unsigned_long(data, sd_message_->GetService_tag(), -1);
    int8_t operation_id = sd_message_->GetOperation_id();
    data.append((const char*)&operation_id, sizeof(operation_id));
    
    encode_field_list(data, sd_message_);
    
    int32_t data_size = data.size();
    int32_t dataLength = ntohl(data.size() + 4);
    _in_progress.append(1,'D');
    _in_progress.append((const char*)&dataLength, sizeof(dataLength));
    _in_progress.append((const char*)&data_size, sizeof(data_size));
    _in_progress.append(data);
    return this->_in_progress;
}

void kt::sdoff_encoder::encode_array(std::string& buffer, kt::field_value_t value) {
    buffer.append(1, ARRAY);
    std::vector<field_value_t> fields = value.GetField_values();
    size_t initial_pos = buffer.size();
    
    std::string tmpbuf;
    encode_unsigned_long(tmpbuf, fields.size(), -1L);
    for(int j = 0; j < fields.size(); j++) {
        encode_field_value(tmpbuf, fields[j]);
    }
    int32_t len = tmpbuf.size();
    buffer.append((const char*)&len, sizeof(len));
    buffer.append(tmpbuf);
}

void kt::sdoff_encoder::encode_field_list(std::string& buffer, kt::field_struct_t field_list) {
    std::list<field_t>& lis = field_list.get_fields();
    for(field_t& field : lis) {
        field_tag_t field_tag = field.GetTag();
        encode_unsigned_long(buffer, field_tag.GetTag(), -1L);
        encode_field_value(buffer, field.GetField_value());
    }
}

void kt::sdoff_encoder::encode_field_value(std::string& buffer, kt::field_value_t value) {
    field_enum_type_t type = value.GetType();
    if(type == field_enum_type_t::ARRAY) {
        encode_array(buffer,value);
    } else if(type == field_enum_type_t::STRUCT) {
        encode_struct(buffer,*value.GetField_struct());
    } else {
        encode_primitive_type(buffer, value, type);
    }
}

void kt::sdoff_encoder::encode_primitive_type(std::string buffer, kt::field_value_t value, kt::field_enum_type_t type) {
    switch (type) {
        case field_enum_type_t::CHAR:
        case field_enum_type_t::INT8:
            buffer.append(1, BYTE);
            buffer.append(1, (char) value.GetInt8_value());
            break;
        case field_enum_type_t::INT16:
            buffer.append(1, INT16);
            encode_signed_long(buffer, value.GetInt16_value());
            break;
        case field_enum_type_t::INT32:
            buffer.append(1, INT32);
            encode_signed_long(buffer, value.GetInt32_value());
            break;
        case field_enum_type_t::INT64:
            buffer.append(1, INT64);
            encode_signed_long(buffer, value.GetInt64_value());
            break;
        case field_enum_type_t::UINT16:
            buffer.append(1, UINT16);
            encode_unsigned_long(buffer, value.GetUInt16_value(), -1);
            break;
        case field_enum_type_t::DATE:
        case field_enum_type_t::UINT32:
            buffer.append(1, UINT32);
            encode_unsigned_long(buffer, value.GetUInt32_value(), -1L);
            break;
        case field_enum_type_t::UINT64:
        case field_enum_type_t::DATETIME:
        case field_enum_type_t::TIMESTAMP:
            buffer.append(1, UINT64);
            encode_unsigned_long(buffer, value.GetUint64_value(), -1L);
            break;
        case field_enum_type_t::OCTETS:
        {
            buffer.append(1, OCTETS);
            std::vector<char> oct = value.GetOctets_value();
            encode_unsigned_long(buffer, oct.size(), -1L);
            for (char c : oct) {
                buffer.append(1, c);
            }
        }
            break;
        case field_enum_type_t::REAL32: {
            buffer.append(1, REAL32);
            float f = value.GetReal32_value();
            buffer.append((const char*)&f, sizeof (f));
        }
            break;
        case field_enum_type_t::REAL64: {
            buffer.append(1, REAL64);
            double d = value.GetReal64_value();
            buffer.append((const char*)&d, sizeof (d));
        }
            break;
        case field_enum_type_t::STRING:
            buffer.append(1, STRING);
            encode_string(buffer, value.GetString_value());
            break;
        case field_enum_type_t::BOOLEAN:
            buffer.append(1, value.GetBoolen_value()? 't':'f');
            break;
        default:
            break;
    }
}

void kt::sdoff_encoder::encode_signed_long(std::string& buffer, long value) {
    long tmp = value;
    char sign = 0x00;

    if (tmp < 0) {
        sign = (char) 0x40;
        tmp = -tmp;
    }

    if (tmp > 0x3F) {
        buffer.append(1, (const char) (STOP_BIT | sign | (tmp & 0x3F)));
        tmp = tmp >> 6;

        while (tmp > BIT_MASK) {
            buffer.append(1, (const char) (STOP_BIT | (tmp & BIT_MASK)));
            tmp = tmp >> 7;
        }

        buffer.append(1, (const char) tmp);
    } else {
        buffer.append(1, (const char) (sign | tmp));
    }
}

void kt::sdoff_encoder::encode_string(std::string& buffer, std::string value) {
    size_t size = buffer.size();
    encode_unsigned_long(buffer, size + 1, -1L);
    buffer.append(value.c_str(), value.size());
    buffer.append(1, '\0');
}

void kt::sdoff_encoder::encode_struct(std::string& buffer, kt::field_struct_t struct_field_list) {
    buffer.append(1, STRUCT);
    
    std::string tmpbuf;
    encode_field_list(tmpbuf, struct_field_list);
    int32_t len = tmpbuf.size();
    buffer.append((const char*)&len, sizeof(len));
    buffer.append(tmpbuf);
}

void kt::sdoff_encoder::encode_unsigned_long(std::string& buffer, long value, long mask) {
    long tmp = value;
    if (tmp < 0) {
        if ((tmp & ~mask) != ~mask) {
            assert(false);
            return;
        }
        tmp = value & mask;
    }
    while ((tmp & 0xffffffffffffff80L) != 0) {
        buffer.append(1, (const char) (STOP_BIT | (tmp & BIT_MASK)));
        tmp = tmp >> 7;
    }
    buffer.append(1, (const char) (tmp & BIT_MASK));
}
