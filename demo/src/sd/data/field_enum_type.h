/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   field_enum_type.h
 * Author: kecy
 *
 * Created on 2018年11月16日, 下午2:00
 */

#ifndef FIELD_ENUM_TYPE_H
#define FIELD_ENUM_TYPE_H

namespace kt {
    enum field_enum_type_t {
        /**
	 * Int16.
	 */
	INT16,
	/**
	 * Int32.
	 */
	INT32,
	/**
	 * Int64.
	 */
	INT64,
	/**
	 * UInt32.
	 */
	UINT32,
	/**
	 * UInt64.
	 */
	UINT64,
	/**
	 * String.
	 */
	STRING,
	/**
	 * UInt16.
	 */
	UINT16,
	/**
	 * UInt8.
	 */
	INT8,
	/**
	 * Char.
	 */
	CHAR,
	/**
	 * Struct.
	 */
	STRUCT,
	/**
	 * Array.
	 */
	ARRAY,
	/**
	 * Double.
	 */
	REAL64,
	/**
	 * Float.
	 */
	REAL32,
	/**
	 * Boolean.
	 */
	BOOLEAN,
	/**
	 * Date.
	 */
	DATE,
	/**
	 * Time.
	 */
	TIME,
	/**
	 * Time stamp.
	 */
	TIMESTAMP,
	/**
	 * Date time.
	 */
	DATETIME,
	/**
	 * Octets.
	 */
	OCTETS,
	/**
	 * Unknown.
	 */
	UNKNOWN
    };
}


#endif /* FIELD_ENUM_TYPE_H */

