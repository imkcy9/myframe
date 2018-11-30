/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   decoder.h
 * Author: kecy
 *
 * Created on 2018年11月23日, 上午10:19
 */

#ifndef DECODER_H
#define DECODER_H
#include <assert.h>
#include <stddef.h>
#include "i_decoder.h"
namespace kt {

    template <typename T>
    class decoder_base_t : public i_decoder {
    public:
        decoder_base_t(size_t buffsize_) :
        _next (NULL),
        _read_pos (buff_),
        _to_read (0)
        //_allocator (buf_size_) 
        {
            
        }
        virtual ~decoder_base_t() {}

        virtual void get_buffer(unsigned char** data_, size_t* size_) {
//            _buf = _allocator.allocate();
//            if (_to_read >= _allocator.size()) {
//                *data_ = _read_pos;
//                *size_ = _to_read;
//                return;
//            }
//
//            *data_ = _buf;
//            *size_ = _allocator.size();
        }

        virtual int decode(const unsigned char *data_, size_t size_, size_t &bytes_used_) {
            bytes_used_ = 0;

            //  In case of zero-copy simply adjust the pointers, no copying
            //  is required. Also, run the state machine in case all the data
            //  were processed.
            if (data_ == _read_pos) {
                assert(size_ <= _to_read);
                _read_pos += size_;
                _to_read -= size_;
                bytes_used_ = size_;

                while (!_to_read) {
                    const int rc =
                            (static_cast<T *> (this)->*_next) (data_ + bytes_used_);
                    if (rc != 0)
                        return rc;
                }
                return 0;
            }

            while (bytes_used_ < size_) {
                //  Copy the data from buffer to the message.
                const size_t to_copy = std::min(_to_read, size_ - bytes_used_);
                // Only copy when destination address is different from the
                // current address in the buffer.
                if (_read_pos != data_ + bytes_used_) {
                    memcpy(_read_pos, data_ + bytes_used_, to_copy);
                }

                _read_pos += to_copy;
                _to_read -= to_copy;
                bytes_used_ += to_copy;
                //  Try to get more space in the message to fill in.
                //  If none is available, return.
                while (_to_read == 0) {
                    // pass current address in the buffer
                    const int rc =
                            (static_cast<T *> (this)->*_next) (data_ + bytes_used_);
                    if (rc != 0)
                        return rc;
                }
            }

            return 0;
        }

        virtual void resize_buffer(std::size_t new_size_) {
            //_allocator.resize(new_size_);
        }
    protected:
        typedef int (T::*step_t) (unsigned char const *);

        void next_step(void *read_pos_, std::size_t to_read_, step_t next_) {
            _read_pos = static_cast<unsigned char *> (read_pos_);
            _to_read = to_read_;
            _next = next_;
        }

        //A &get_allocator() {
        //    return _allocator;
        //}
    private:
        step_t _next;

        //  Where to store the read data.
        unsigned char *_read_pos;
        unsigned char buff_[8192];
        //  How much data to read before taking next step.
        std::size_t _to_read;

        //  The duffer for data to decode.
        //A _allocator;
        unsigned char *_buf;

    };
}


#endif /* DECODER_H */

