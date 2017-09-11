/* 
 * File:   epoll.h
 * Author: kcy
 *
 * Created on 2015年12月7日, 上午11:06
 */

#ifndef EPOLL_H
#define	EPOLL_H
#include <sys/epoll.h>
#include "thread.h"
#include "poller_base.h"
#include <vector>


struct i_poll_events;

class epoll_t : public poller_base_t
{
public:
    typedef void* handle_t;
    epoll_t();
    ~epoll_t();

    //"poller" concept
    handle_t add_fd(int fd_, i_poll_events *events_);
    void rm_fd(handle_t handle_);
    void set_pollin(handle_t handle_);
    void reset_pollin(handle_t handle_);
    void set_pollout(handle_t handle_);
    void reset_pollout(handle_t handle_);
    void start();
    void stop();

    static int max_fds();

private:
    static void worker_routine(void *arg_); //工作线程 
    void loop();//事件循环
    int epoll_fd;//主poll 描述符

    struct poll_entry_t
    {
        int fd;
        epoll_event ev;
        i_poll_events *events;
    };

    typedef std::vector<poll_entry_t*> retired_t;
    retired_t retired;
    //If true, thread is in the process of shutting down.
    bool stopping;

    thread_t worker;
    epoll_t(const epoll_t&);
    const epoll_t& operator=(const epoll_t&);
};

typedef epoll_t poller_t;



#endif	/* EPOLL_H */

