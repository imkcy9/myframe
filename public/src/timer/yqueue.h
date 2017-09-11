/* 
 * File:   yqueue.h
 * Author: kcy
 *
 * Created on 2016年1月25日, 下午1:29
 */

#ifndef YQUEUE_H
#define	YQUEUE_H

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include "atomic_ptr.h"

template <typename T, int N> class yqueue_t
{
public:
    inline yqueue_t()
    {
            begin_chunk = (chunk_t *)malloc(sizeof(chunk_t));
            assert(begin_chunk);
            begin_pos = 0;
            end_chunk = begin_chunk;
            end_pos = 0;
            back_chunk = NULL;
            back_chunk = 0;
    }

    inline ~yqueue_t()
    {
            while (true)
            {
                    if (begin_chunk == end_chunk)
                    {
                            free(begin_chunk);
                            break;
                    }
                    chunk_t *o = begin_chunk;
                    begin_chunk = begin_chunk->next;
                    free(o);
            }
    }

    inline T &front()
    {
            return begin_chunk->values[begin_pos];
    }

    inline T &back()
    {
            return back_chunk->values[back_pos];
    }

    inline void push()
    {
            back_chunk = end_chunk;
            back_pos = end_pos;

            if (++end_pos != N)
            {
                    return ;
            }

            chunk_t *sc = spare_chunk.xchg(NULL);
            if (sc)
            {
                    end_chunk->next = sc;
                    sc->prev = end_chunk;
            }else{
                    end_chunk->next = (chunk_t *)malloc(sizeof(chunk_t));
                    assert(end_chunk->next);
                    end_chunk->next->prev = end_chunk;
            }
            end_chunk = end_chunk->next;
            end_pos = 0;
    }

    inline void unpush ()
    {
            //  First, move 'back' one position backwards.
            if (back_pos)
                    --back_pos;
            else {
                    back_pos = N - 1;
                    back_chunk = back_chunk->prev;
            }

            //  Now, move 'end' position backwards. Note that obsolete end chunk
            //  is not used as a spare chunk. The analysis shows that doing so
            //  would require free and atomic operation per chunk deallocated
            //  instead of a simple free.
            if (end_pos)
                    --end_pos;
            else {
                    end_pos = N - 1;
                    end_chunk = end_chunk->prev;
                    free (end_chunk->next);
                    end_chunk->next = NULL;
            }
    }

    //  Removes an element from the front end of the queue.
    inline void pop ()
    {
            if (++ begin_pos == N) {
                    chunk_t *o = begin_chunk;
                    begin_chunk = begin_chunk->next;
                    begin_chunk->prev = NULL;
                    begin_pos = 0;

                    //  'o' has been more recently used than spare_chunk,
                    //  so for cache reasons we'll get rid of the spare and
                    //  use 'o' as the spare.
                    chunk_t *cs = spare_chunk.xchg (o);
                    free (cs);
            }
    }

private:
    struct chunk_t
    {
            T values[N];
            chunk_t *prev;
            chunk_t *next;
    };

    chunk_t *begin_chunk;
    int begin_pos;
    chunk_t *back_chunk;
    int back_pos;
    chunk_t *end_chunk;
    int end_pos;

    atomic_ptr_t<chunk_t> spare_chunk;

    yqueue_t (const yqueue_t&);
    const yqueue_t &operator = (const yqueue_t&);
            
};



#endif	/* YQUEUE_H */

