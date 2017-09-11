/* 
 * File:   epoll.cpp
 * Author: kcy
 *
 * Created on 2015年12月7日, 下午1:04
 */

#include "epoll.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <new>
#include <assert.h>
#include "i_poll_events.h"
#include <errno.h>

epoll_t::epoll_t() :
stopping(false)
{
    epoll_fd = epoll_create(1);
    assert(epoll_fd != -1);
}

epoll_t::~epoll_t()
{
    // Wait till the worker thread exits.
    worker.stop();
    close(epoll_fd);
    for(retired_t::iterator it = retired.begin(); it != retired.end(); ++it)
    {
        delete *it;
    }
}

epoll_t::handle_t epoll_t::add_fd(int fd_,i_poll_events *events_)
{
    poll_entry_t *pe = new (std::nothrow) poll_entry_t;
    assert(pe);
    memset(pe,0,sizeof(poll_entry_t));
    
    pe->fd = fd_;
    pe->ev.events = 0;
    pe->ev.data.ptr = pe;
    pe->events = events_;
    
    int rc = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd_,&pe->ev);
    assert(rc != -1);
    
    adjust_load(1);
    return pe;
}

void epoll_t::rm_fd(handle_t handle_)
{
    poll_entry_t *pe = (poll_entry_t*)handle_;
    int rc = epoll_ctl(epoll_fd, EPOLL_CTL_DEL,pe->fd,&pe->ev);
    assert(rc != -1);
    pe->fd = -1;//retired_fd 
    retired.push_back(pe);
    adjust_load(-1);
}

void epoll_t::set_pollin(handle_t handle_)
{
    poll_entry_t *pe = (poll_entry_t*)handle_;
    pe->ev.events |= EPOLLIN;
    int rc = epoll_ctl(epoll_fd,EPOLL_CTL_MOD,pe->fd,&pe->ev);
    assert(rc != -1);
}

void epoll_t::reset_pollin(handle_t handle_)
{
    poll_entry_t *pe = (poll_entry_t*)handle_;
    pe->ev.events &= ~((short)EPOLLIN);
    int rc = epoll_ctl(epoll_fd,EPOLL_CTL_MOD,pe->fd,&pe->ev);
    assert(rc != -1);
}

void epoll_t::set_pollout (handle_t handle_)
{
    poll_entry_t *pe = (poll_entry_t*) handle_;
    pe->ev.events |= EPOLLOUT;
    int rc = epoll_ctl (epoll_fd, EPOLL_CTL_MOD, pe->fd, &pe->ev);
    assert (rc != -1);
}

void epoll_t::reset_pollout (handle_t handle_)
{
    poll_entry_t *pe = (poll_entry_t*) handle_;
    pe->ev.events &= ~((short) EPOLLOUT);
    int rc = epoll_ctl (epoll_fd, EPOLL_CTL_MOD, pe->fd, &pe->ev);
    assert (rc != -1);
}

void epoll_t::start()
{
    worker.start(worker_routine,this);
}

void epoll_t::stop()
{
    stopping = true;
}

int epoll_t::max_fds()
{
    return -1;
}

void epoll_t::worker_routine(void *arg_)
{
    ((epoll_t*)arg_)->loop();
}

void epoll_t::loop()
{
    epoll_event ev_buf[256];//max io events;

    while(!stopping)
    {
        int timeout = (int)execute_timers();//执行定时任务 
        int n = epoll_wait(epoll_fd,&ev_buf[0],256,timeout?timeout:-1);//wait for events.
        if(n == -1)
        {
            assert(errno == EINTR);
            continue;
        }
        
        for(int i = 0; i < n; i++)
        {
            poll_entry_t *pe = ((poll_entry_t*)ev_buf[i].data.ptr);
            
            if(pe->fd == -1)//retired fd;
                continue;
            if(ev_buf[i].events & (EPOLLERR | EPOLLHUP))
                pe->events->in_event();
            if (pe->fd == -1)
               continue;
            if (ev_buf [i].events & EPOLLOUT)
                pe->events->out_event ();
            if (pe->fd == -1)
                continue;
            if (ev_buf [i].events & EPOLLIN)
                pe->events->in_event ();
        }
        //  Destroy retired event sources.
        for (retired_t::iterator it = retired.begin (); it != retired.end ();
              ++it)
            delete *it;
        retired.clear ();
    }
}