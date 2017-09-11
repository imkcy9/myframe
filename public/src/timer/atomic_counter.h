/* 
 * File:   atomic_counter.h
 * Author: kcy
 *
 * Created on 2015年12月7日, 上午10:08
 */

#ifndef ATOMIC_COUNTER_H
#define	ATOMIC_COUNTER_H
#include <stddef.h>
namespace kcy
{
    //原子计数器
    class atomic_counter_t
    {
    public:
        inline atomic_counter_t(uint32_t value_ = 0) :
        value(value_)
        {
        }
        
        inline ~atomic_counter_t()
        {
            
        }
        //原子增加
        inline uint32_t add(uint32_t increment_)
        {
            uint32_t old_value;
                    __asm__ volatile (
            "lock; xadd %0, %1 \n\t"
            : "=r" (old_value), "=m" (value)
            : "0" (increment_), "m" (value)
            : "cc", "memory");
            
                    return old_value;
        }
        //原子减
        inline bool sub(uint32_t decrement)
        {
            uint32_t oldval = -decrement;
            volatile uint32_t *val = &value;
            
            __asm__ volatile ("lock; xaddl %0,%1"
                : "=r" (oldval), "=m" (*val)
                : "0" (oldval), "m" (*val)
                : "cc", "memory");
            
            return oldval != decrement;
        }
            
        inline uint32_t get()
        {
            return value;
        }
    private:
        volatile uint32_t value;
        
        atomic_counter_t(const atomic_counter_t&);
        const atomic_counter_t& operator=(const atomic_counter_t&);
    };
}


#endif	/* ATOMIC_COUNTER_H */

