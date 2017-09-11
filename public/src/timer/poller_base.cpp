#include "poller_base.h"
#include "i_poll_events.h"
#include <assert.h>


poller_base_t::poller_base_t()
{
    
}
poller_base_t::~poller_base_t ()
{
    // 确保关闭时计数为0
    assert(load.get() == 0);
}

int poller_base_t::get_load()
{
    return load.get();
}

void poller_base_t::adjust_load(int amount_)
{
    if(amount_ > 0)
        load.add(amount_);
    else if(amount_ < 0)
        load.sub(-amount_);
}

void poller_base_t::add_timer(int timeout_, i_poll_events *sink_, int id_, bool loop_)
{
    uint64_t expiration = clock.now_ms() + timeout_;
    timer_info_t info = {sink_, id_, timeout_, loop_};
    timers.insert(timers_t::value_type(expiration,info));
}

void poller_base_t::cancel_timer(i_poll_events *sink_, int id_)
{
    for(timers_t::iterator it = timers.begin(); it != timers.end(); ++it)
    {
        if(it->second.sink == sink_ && it->second.id == id_)
        {
            timers.erase(it);
            return;
        }
    }
    assert(false); //没有定时器
}

void poller_base_t::cancel_all_timer()
{
    for(timers_t::iterator it = timers.begin(); it != timers.end(); ++it)
    {
        timers.erase(it);
    }
}

//执行定时器
uint64_t poller_base_t::execute_timers()
{
    if(timers.empty())
        return 0;
    //Get the current time.
    uint64_t current = clock.now_ms();
    
    timers_t::iterator it = timers.begin();
    while(it != timers.end())
    {
        //由于MAP默认按KEY值从小到大排序，所以每遍历到一个未触发的时间事件时，
        //这个事件时间是最小最接近触发时间的，直接返回这个时间作为EPOLL的等待时间即可。
        if(it->first > current) //定时时间大于当前时间?
            return it->first - current;//未触发
        
        it->second.sink->timer_event(it->second.id); //触发定时器事件
        
        timers_t::iterator o = it;
        if(o->second.loop)
        {
            //如果事件是循环的就重新计算下次的时间并加入MAP
            timers.insert(timers_t::value_type(clock.now_ms() + o->second.timeout, o->second));
        }
        ++it;
        timers.erase(o);//delete the event;
    }
    return 0;
}