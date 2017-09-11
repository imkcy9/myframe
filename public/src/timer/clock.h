/* 
 * File:   clock.h
 * Author: kcy
 *
 * Created on 2015年12月7日, 上午9:30
 */

#ifndef CLOCK_H
#define	CLOCK_H
#include <stdint.h>
namespace kcy
{
    class clock_t
    {
    public:
        clock_t();
        ~clock_t();
        
        static uint64_t rdtsc();
        
        static uint64_t now_us();
        
        uint64_t now_ms();
    private:
        uint64_t last_tsc;
        
        uint64_t last_time;
        
        clock_t(const clock_t&);
        const clock_t& operator=(const clock_t&);
    };
}


#endif	/* CLOCK_H */

