/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "sdoff_decoder.h"
#include <limits.h>
#include <netinet/in.h>
#include <limits>
#include "wire.hpp"
#include "../sd/data/field_struct.h"
#include "value_type.h"
#include "../sd/data/sd_message.h"
#include "sd/data/sd_constants.h"
#include <iostream>

void decode_field_list(kt::field_struct_t& field_struct_, unsigned char **buffer_, int length);
std::vector<kt::field_value_t> fast_decode_array(unsigned char **buffer_);

inline char fast_decode_char(unsigned char **buffer_) {
    char result = 0;
    char shift = 0;
    char b = -1;
    while (b < 0) {
        b = *(*buffer_);
        ++(*buffer_);
        result += ((b & 0x7F)) << shift;
        shift += 7;

        break;
    }
    return result;
}

inline int fast_decode_int(unsigned char **buffer_) {
    int result = 0;
    char shift = 0;
    char b = -1;
    while (b < 0) {
        b = *(*buffer_);
        ++(*buffer_);
        result += ((b & 0x7F)) << shift;
        shift += 7;
    }
    return result;
}

inline float fast_decode_float(unsigned char **buffer_) {
    float* f = (float*)(*buffer_);
    (*buffer_) += 4;
    return *f;
}
inline double fast_decode_double(unsigned char **buffer_) {
    double* d = (double*)(*buffer_);
    (*buffer_) += 8;
    return *d;
}


inline std::string fast_decode_string(unsigned char **buffer_) {
    int stringlen = fast_decode_int(buffer_) - 1;
    if (stringlen <= 0) {
        if (stringlen == 0) {
            fast_decode_char(buffer_);
        }
        return "";
    }
    std::string chars = (const char*) (*buffer_);
    *buffer_ = (*buffer_) + chars.size() + 1;
    return chars;
}

inline long fast_decode_long(unsigned char **buffer_, bool signed_) {
    long result = 0;
    char shift = 0;
    char b = -1;
    bool neg = false;

    while (b < 0) {
        b = *(*buffer_);
        ++(*buffer_);
        if (signed_ && shift == 0) {
            neg = ((b & 0x40) == 0x40);
            result += (b & 0x3F);
            shift = 6;
        } else {
            if (signed_ && shift == 62) {
                long lastPart = (b & 0x7F);
                if (lastPart == 0L || lastPart == 1L) {
                    result += lastPart << shift;
                } else if (neg && lastPart == 2L && result == 0L) {
                    return (std::numeric_limits<long>::min)();
                } else {
                    //throw new RuntimeException("Value overflow, value outside range of long");
                }
            } else {
                result += ((long) (b & 0x7F)) << shift;
                shift += 7;
            }
        }
    }

    if (neg && result < 0) {
        //throw new RuntimeException("Value overflow, value > " + Long.MAX_VALUE);
    }

    return neg ? -result : result;
}



inline void decode_field_value(kt::field_value_t& value, unsigned char **buffer_) {
    char value_type = fast_decode_char(buffer_);
    switch (value_type) {
        case kt::FIELD_BYTE:
            value = kt::field_value_t::value_of(fast_decode_char(buffer_));
            break;
        case kt::FIELD_STRING:
            value = kt::field_value_t::value_of(fast_decode_string(buffer_));
            std::cout << value.GetString_value() << std::endl;
            break;
        case kt::FIELD_STRUCT: {
            int length = fast_decode_int(buffer_);
            kt::field_struct_t field_struct;
            decode_field_list(field_struct, buffer_, length);
            value = kt::field_value_t::value_of(&field_struct);
        }
            break;
        case kt::FIELD_INT16:
            value = kt::field_value_t::value_of((short)fast_decode_long(buffer_, true), kt::field_enum_type_t::INT16);
            break;
        case kt::FIELD_UINT16:
            value = kt::field_value_t::value_of(fast_decode_int(buffer_), kt::field_enum_type_t::UINT16);
            break;
        case kt::FIELD_UINT32:
            value = kt::field_value_t::value_of(fast_decode_long(buffer_, false), kt::field_enum_type_t::UINT32);
            break;
        case kt::FIELD_INT64:
            value = kt::field_value_t::value_of(fast_decode_long(buffer_, true), kt::field_enum_type_t::INT64);
            break;
        case kt::FIELD_UINT64:
            value = kt::field_value_t::value_of(fast_decode_long(buffer_, false), kt::field_enum_type_t::UINT64);
            break;
        case kt::FIELD_REAL32:
            value = kt::field_value_t::value_of(fast_decode_float(buffer_));
            break;
        case kt::FIELD_REAL64:
            value = kt::field_value_t::value_of(fast_decode_double(buffer_));
            break;
        case kt::FIELD_TRUE:
            value = kt::field_value_t::value_of(true);
            break;
        case kt::FIELD_FALSE:
            value = kt::field_value_t::value_of(false);
            break;
        case kt::FIELD_ARRAY:
            value = kt::field_value_t::value_of(fast_decode_array(buffer_));
            break;
        default:
            break;
    }
}

inline void decode_field_list(kt::field_struct_t& field_struct_, unsigned char **buffer_, int length) {
    unsigned char * max_position = (*buffer_) + length;
    while ((*buffer_) < max_position) {
        int tag = fast_decode_long(buffer_, false);
        if (tag <= 0) {
            break;
        }

        kt::field_tag_t fieldtag = kt::field_tag_t::value_of(tag);
        kt::field_value_t fieldvalue;
        decode_field_value(fieldvalue, buffer_);

        if (fieldtag.GetField_type() == kt::UNKNOWN) {
            //todo;
        }

        field_struct_.add(fieldtag, fieldvalue);
    }
}

inline std::vector<kt::field_value_t> fast_decode_array(unsigned char **buffer_) {
    int date_len = *((int32_t*)(*buffer_));
    *buffer_ += 4;
    unsigned char * start_pos = *buffer_;
    int count = fast_decode_int(buffer_);
    
    std::vector<kt::field_value_t> vecs;
    vecs.reserve(count);
    for(int i = 0; i < count; i++) {
        kt::field_value_t value;
        decode_field_value(value, buffer_);
        vecs[i] = value;
    }
    *buffer_ = start_pos + date_len;
    return vecs;
}


kt::sdoff_decoder_t::sdoff_decoder_t()
:kt::sdoff_decoder_t(8192,8192)
{
}

kt::sdoff_decoder_t::sdoff_decoder_t(size_t bufsize_, int64_t maxmsgsize_)
: decoder_base_t<kt::sdoff_decoder_t>(bufsize_),
_zero_copy(false),
_max_msg_size(maxmsgsize_),
_sd_message(NULL){
    _in_progress.rebuild();

    //  At the beginning, read six byte and go to one_byte_size_ready state.
    next_step(&_tmpbuf, 6, &sdoff_decoder_t::six_byte_size_ready);
}

kt::sdoff_decoder_t::~sdoff_decoder_t() {

}

int kt::sdoff_decoder_t::flags_ready(const unsigned char*) {
    //  Store the flags from the wire into the message structure.
    //_in_progress.set_flags (_tmpbuf[0] & msg_t::more);

    next_step(_in_progress.data(), _in_progress.size(),
            &sdoff_decoder_t::message_ready);

    return 0;
}

int kt::sdoff_decoder_t::message_ready(const unsigned char*) {
    //  Message is completely read. Push it further and start reading
    //  new message. (in_progress is a 0-byte message after this point.)
    next_step(sd_head, 9, &sdoff_decoder_t::sd_head_ready);
    return 1;
}

int kt::sdoff_decoder_t::sd_head_ready(const unsigned char*) {
    const unsigned char* pos = (const unsigned char*) sd_head;
    int32_t& l = *((int32_t*)(pos + 1));
    int32_t ll = htonl(l);
    int32_t sdlen = *(pos + 5);
    _in_progress.rebuild(sdlen);
    next_step(_in_progress.data(), _in_progress.size(), &sdoff_decoder_t::sd_message_ready);
    return 0;
}

int kt::sdoff_decoder_t::sd_message_ready(const unsigned char*) {
    unsigned char* buf = (unsigned char *) _in_progress.data();
    unsigned char* start = buf;
    int messageTag = fast_decode_int(&buf);
    int serviceTag = fast_decode_int(&buf);
    int operationID = fast_decode_int(&buf);

    kt::field_struct_t field_struct;
    decode_field_list(field_struct, &buf, _in_progress.size() - (buf - start));
    
    if(_sd_message)
        delete _sd_message;
    _sd_message = new kt::sd_message_t(messageTag, serviceTag, operationID, field_struct);

    next_step(sd_head, 9, &sdoff_decoder_t::sd_head_ready);
    
    if(messageTag == kt::sd_constants_t::MessageTypes::LoginReq) {
        //TODO
        return 2;
    }
    return 3;
}

int kt::sdoff_decoder_t::six_byte_size_ready(const unsigned char*) {
    //  First byte of size is read. If it is UCHAR_MAX (0xff) read 8-byte size.
    //  Otherwise allocate the buffer for message data and read the
    //  message data into it.

    //  There has to be at least one byte (the flags) in the message).

    size_t len = ntohl(_tmpbuf.len);
//    if (_max_msg_size >= 0
//            && static_cast<int64_t> (len) > _max_msg_size) {
//        errno = EMSGSIZE;
//        return -1;
//    }


    //_in_progress.rebuild(*_tmpbuf - 1);
    _in_progress.rebuild(len);
    next_step(_in_progress.data(), len, &sdoff_decoder_t::message_ready);

    return 0;
}

//int kt::sdoff_decoder_t::size_ready(uint64_t size_, const unsigned char*) {
//
//}



