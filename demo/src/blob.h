/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   blob.h
 * Author: kecy
 *
 * Created on 2018年11月27日, 下午3:56
 */

#ifndef BLOB_H
#define BLOB_H

#include <stdlib.h>
#include <string.h>
#include <algorithm>

namespace kt {

    struct blob_t {

    blob_t () : _data (0), _size (0), _owned (true) {}

    explicit blob_t (const size_t size_) :
        _data (static_cast<unsigned char *> (malloc (size_))),
        _size (size_),
        _owned (true)
    {

    }

    blob_t (const unsigned char *const data_, const size_t size_) :
        _data (static_cast<unsigned char *> (malloc (size_))),
        _size (size_),
        _owned (true)
    {
        memcpy (_data, data_, size_);
    }

//    blob_t (unsigned char *const data_, const size_t size_, reference_tag_t) :
//        _data (data_),
//        _size (size_),
//        _owned (false)
//    {
//    }

    size_t size () const { return _size; }

    const unsigned char *data () const { return _data; }

    unsigned char *data () { return _data; }

    bool operator< (blob_t const &other_) const
    {
        const int cmpres =
          memcmp (_data, other_._data, std::min (_size, other_._size));
        return cmpres < 0 || (cmpres == 0 && _size < other_._size);
    }

    void set_deep_copy (blob_t const &other_)
    {
        clear ();
        _data = static_cast<unsigned char *> (malloc (other_._size));
        _size = other_._size;
        _owned = true;
        memcpy (_data, other_._data, _size);
    }

    void set (const unsigned char *const data_, const size_t size_)
    {
        clear ();
        _data = static_cast<unsigned char *> (malloc (size_));
        _size = size_;
        _owned = true;
        memcpy (_data, data_, size_);
    }

    //  Empties a blob_t.
    void clear ()
    {
        if (_owned) {
            free (_data);
        }
        _data = 0;
        _size = 0;
    }

    ~blob_t ()
    {
        if (_owned) {
            free (_data);
        }
    }
    
    blob_t (const blob_t &other) : _owned (false) { set_deep_copy (other); }
    blob_t &operator= (const blob_t &other)
    {
        if (this != &other) {
            clear ();
            set_deep_copy (other);
        }
        return *this;
    }
        
    //--------------------- c++11 Move semantics
//    blob_t (const blob_t &) = delete;
//    blob_t &operator= (const blob_t &) = delete;
//
//    blob_t (blob_t &&other_) noexcept : _data (other_._data),
//                                            _size (other_._size),
//                                            _owned (other_._owned)
//    {
//        other_._owned = false;
//    }
//    
//    blob_t &operator= (blob_t &&other_) noexcept
//    {
//        if (this != &other_) {
//            clear ();
//            _data = other_._data;
//            _size = other_._size;
//            _owned = other_._owned;
//            other_._owned = false;
//        }
//        return *this;
//    }
    
        
    private:
        unsigned char *_data;
        size_t _size;
        bool _owned;
    };
}



#endif /* BLOB_H */

