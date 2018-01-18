//
// Created by chengye.ke on 2018/1/16.
//

#ifndef DEMO_DECODER_BASE_H
#define DEMO_DECODER_BASE_H
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <zmq.hpp>

template <typename T>
class decoder_base {
public:

    explicit decoder_base () :
    next (NULL),
    read_pos (NULL),
    to_read (0)
            {
                    //buf = allocator->allocate ();
            }

    //  The destructor doesn't have to be virtual. It is made virtual
    //  just to keep ICC and code checking tools from complaining.
    virtual ~decoder_base ()
    {
        //allocator->deallocate ();
    }

    int decode (const unsigned char *data_, std::size_t size_,
                std::size_t &bytes_used_)
    {
        bytes_used_ = 0;

        //  In case of zero-copy simply adjust the pointers, no copying
        //  is required. Also, run the state machine in case all the data
        //  were processed.
        if (data_ == read_pos) {
            ZMQ_ASSERT (size_ <= to_read);
            read_pos += size_;
            to_read -= size_;
            bytes_used_ = size_;

            while (!to_read) {
                const int rc =
                        (static_cast <T *> (this)->*next) (data_ + bytes_used_);
                if (rc != 0)
                    return rc;
            }
            return 0;
        }

        while (bytes_used_ < size_) {
            //  Copy the data from buffer to the message.
            const size_t to_copy = std::min (to_read, size_ - bytes_used_);
            // Only copy when destination address is different from the
            // current address in the buffer.
            if (read_pos != data_ + bytes_used_) {
                memcpy (read_pos, data_ + bytes_used_, to_copy);
            }

            read_pos += to_copy;
            to_read -= to_copy;
            bytes_used_ += to_copy;
            //  Try to get more space in the message to fill in.
            //  If none is available, return.
            while (to_read == 0) {
                // pass current address in the buffer
                const int rc =
                        (static_cast <T *> (this)->*next) (data_ + bytes_used_);
                if (rc != 0)
                    return rc;
            }
        }

        return 0;
    }

protected:

    //  Prototype of state machine action. Action should return false if
    //  it is unable to push the data to the system.
    typedef int (T:: *step_t) (unsigned char const *);

    //  This function should be called from derived class to read data
    //  from the buffer and schedule next state machine action.
    void next_step (void *read_pos_, std::size_t to_read_, step_t next_)
    {
        read_pos = static_cast <unsigned char*> (read_pos_);
        to_read = to_read_;
        next = next_;
    }

private:

    //  Next step. If set to NULL, it means that associated data stream
    //  is dead. Note that there can be still data in the process in such
    //  case.
    step_t next;

    //  Where to store the read data.
    unsigned char *read_pos;

    //  How much data to read before taking next step.
    std::size_t to_read;

    unsigned char *buf;
};
#endif //DEMO_DECODER_BASE_H
