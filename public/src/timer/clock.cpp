#include "clock.h"
#include <stddef.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
uint64_t clock_precision = 1000000;
kcy::clock_t::clock_t() :
    last_tsc(rdtsc()),
    last_time(now_us()/1000)
{
    
}

kcy::clock_t::~clock_t()
{
    
}

uint64_t kcy::clock_t::now_us()
{
    struct timespec tv;
    int rc = clock_gettime(CLOCK_MONOTONIC,&tv); //CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
    if(rc != 0)
    {
        struct timeval tv;
        int rc = gettimeofday(&tv,NULL);
        assert(rc == 0);
        return (tv.tv_sec *(uint64_t)1000000 + tv.tv_usec);
    }
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_nsec / 1000); 
}

uint64_t kcy::clock_t::now_ms()
{
    uint64_t tsc = rdtsc();
    if(!tsc)
    {
        return now_us() / 1000;
    }
    if (tsc - last_tsc <= (clock_precision / 2) && tsc >= last_tsc)
        return last_time;
    last_tsc = tsc;
    last_time = now_us () / 1000;
    return last_time;
}

uint64_t kcy::clock_t::rdtsc ()
{
    uint32_t low, high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    return (uint64_t) high << 32 | low;
    return 0;
}