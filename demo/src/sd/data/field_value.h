/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field_value.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午2:01
 */

#ifndef FIELD_VALUE_H
#define FIELD_VALUE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <assert.h>
#include "field_enum_type.h"
//#include "field_struct.h"
namespace kt {
    class field_struct_t;
    class field_value_t {
    public:
        field_value_t() {
            _field_struct = NULL;
        }
        field_value_t(field_enum_type_t type) {
            this->_type = type;
            _field_struct = NULL;
        }

        virtual ~field_value_t();

        bool IsBoolen_value() const {
            return _boolen_value;
        }

        field_struct_t* GetField_struct() const {
            return _field_struct;
        }

        std::vector<field_value_t>& GetField_values() {
            return _field_values;
        }

        int8_t GetInt8_value() const {
            return _int8_value;
        }

        int32_t GetInt32_value() const {
            return _int32_value;
        }

        int64_t GetInt64_value() const {
            return _int64_value;
        }

        float GetReal32_value() const {
            return _real32_value;
        }

        double GetReal64_value() const {
            return _real64_value;
        }

        std::string GetString_value() const {
            return _string_value;
        }

        field_enum_type_t GetType() const {
            return _type;
        }

        uint64_t GetUint64_value() const {
            return _uint64_value;
        }

        static field_value_t value_of(long value, field_enum_type_t type);
        
        static field_value_t value_of(char value) {
            field_value_t des_field_value(field_enum_type_t::CHAR);
            des_field_value._int8_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(int value) {
            field_value_t des_field_value(field_enum_type_t::INT64);
            des_field_value._int64_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(float value) {
            field_value_t des_field_value(field_enum_type_t::REAL32);
            des_field_value._real32_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(double value) {
            field_value_t des_field_value(field_enum_type_t::REAL64);
            des_field_value._real64_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(std::string value) {
            field_value_t des_field_value(field_enum_type_t::STRING);
            des_field_value._string_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(bool value) {
            field_value_t des_field_value(field_enum_type_t::BOOLEAN);
            des_field_value._boolen_value = value;
            return des_field_value;
        }
        
        static field_value_t value_of(std::vector<field_value_t> value) {
            field_value_t des_field_value(field_enum_type_t::ARRAY);
            des_field_value._field_values = value;
            return des_field_value;
        }
        
        static field_value_t value_of(field_struct_t* value);
    public:
        field_enum_type_t _type;
        //byte _byte_value;
        int8_t _int8_value;
        int32_t _int32_value;
        int64_t _int64_value;
        std::string _string_value;
        field_struct_t* _field_struct;
        std::vector<field_value_t> _field_values;
        float _real32_value;
        double _real64_value;
        uint64_t _uint64_value;
        bool _boolen_value;

    };
}


#endif /* FIELD_VALUE_H */

