/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "field_value.h"
#include "field_struct.h"

kt::field_value_t::~field_value_t() {
    if (_field_struct)
        delete _field_struct;
}

kt::field_value_t kt::field_value_t::value_of(long value, field_enum_type_t type) {
    field_value_t des_field_value(type);
    switch (type) {
        case field_enum_type_t::BOOLEAN:
            des_field_value._boolen_value = value;
            break;
        case field_enum_type_t::INT8:
        case field_enum_type_t::CHAR:
            des_field_value._int8_value = value;
            break;
        case field_enum_type_t::INT16:
        case field_enum_type_t::INT32:
            des_field_value._int32_value = value;
            break;
        case field_enum_type_t::DATE:
        case field_enum_type_t::UINT32:
        case field_enum_type_t::INT64:
            des_field_value._int64_value = value;
            break;
        case field_enum_type_t::DATETIME:
        case field_enum_type_t::UINT64:
            des_field_value._uint64_value = value;
            break;
        case field_enum_type_t::REAL32:
            des_field_value._real32_value;
            break;
        case field_enum_type_t::REAL64:
            des_field_value._real64_value;
            break;
        case field_enum_type_t::STRING:
            des_field_value._string_value = value;
            break;
//        case field_enum_type_t::STRUCT:
//            des_field_value._field_struct = value;
//            break;
//        case field_enum_type_t::ARRAY:
//            des_field_value._field_values = value;
            break;
        default:
            assert(false);
            break;
    }

    return des_field_value;
}

kt::field_value_t kt::field_value_t::value_of(field_struct_t* value) {
    field_value_t des_field_value(field_enum_type_t::STRUCT);
    des_field_value._field_struct = new field_struct_t(value->get_fields());
    return des_field_value;
}