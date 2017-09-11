/* 
 * File:   atomic_ptr.h
 * Author: kcy
 *
 * Created on 2016年1月25日, 下午1:10
 */

#ifndef ATOMIC_PTR_H
#define	ATOMIC_PTR_H

#include <stddef.h>

template <typename T> class atomic_ptr_t
{
public:
    inline atomic_ptr_t()
    {
        ptr = NULL;
    }
    
    inline ~atomic_ptr_t()
    {
        
    }
    
    inline void set(T* ptr_)
    {
        this->ptr = ptr_;
    }
    
    inline T* xchg(T* val_)
    {
        T* old;
        __asm__ volatile (
        "lock; xchg %0, %2"
        : "=r" (old), "=m" (ptr)
        : "m" (ptr), "0" (val_)
        );
        return old;
    }
    
    //if (cmp_ == ptr)
    //      ptr = val_;
    //return ptr;
    inline T* cas(T* cmp_, T* val_)
    {
        T* old;
        __asm__ volatile (
        "lock; cmpxchg %2, %3"
        : "=a" (old), "=m" (ptr)
        : "r" (val_), "m" (ptr), "0" (cmp_)
        : "cc");
        return old;
    }
        
private:
    volatile T* ptr;
    
    atomic_ptr_t(const atomic_ptr_t&);
    const atomic_ptr_t& operator = (const atomic_ptr_t&);
};


#endif	/* ATOMIC_PTR_H */

