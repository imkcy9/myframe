#include "signaler.h"
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <stdint.h>
#include <poll.h>
#include <fcntl.h>
static int sleep_ms(unsigned int ms_)
{
    if(ms_ == 0)
        return 0;
    return usleep(ms_ * 1000);
}

static int close_wait_ms(int fd_, unsigned int max_ms_ = 2000)
{
    unsigned int ms_so_far = 0;
    unsigned int step_ms = max_ms_ /10;
    if(step_ms < 1)
        step_ms = 1;
    
    if(step_ms > 100)
        step_ms = 100;
    int rc = 0;
    do
    {
        if(rc == -1 && errno == EAGAIN)
        {
            sleep_ms(step_ms);
            ms_so_far += step_ms;
        }
        rc = close(fd_);
    }while(ms_so_far < max_ms_ && rc == -1 && errno == EAGAIN);
    return 0;
}

signaler_t::signaler_t()
{
    if(make_fdpair(&r,&w) == 0)
    {
        int flags = fcntl (r, F_GETFL, 0);
        if (flags == -1)
            flags = 0;
        int rc = fcntl (r, F_SETFL, flags | O_NONBLOCK);
        assert (rc != -1);
        
        flags = fcntl (w, F_GETFL, 0);
        if (flags == -1)
            flags = 0;
        rc = fcntl (w, F_SETFL, flags | O_NONBLOCK);
        assert (rc != -1);
    }
}

signaler_t::~signaler_t()
{
    int rc = close_wait_ms (r);
    assert (rc == 0);
}

int signaler_t::get_fd()
{
    return r;
}

void signaler_t::send()
{
    const uint64_t inc = 1;
    ssize_t sz = write(w,&inc,sizeof(inc));
    assert(sz == sizeof(inc));
}

int signaler_t::wait(int timeout_)
{
    struct pollfd pfd;
    pfd.fd = r;
    pfd.events = POLLIN;
    int rc = poll(&pfd,1,timeout_);
    if(rc < 0)
    {
        assert(errno == EINTR);
        return -1;
    }else if(0 == rc)
    {
        errno = EAGAIN;
        return -1;
    }
    
    assert(rc == 1);
    assert(pfd.events & POLLIN);
    return 0;
}

void signaler_t::recv()
{
    uint64_t dummy;
    ssize_t sz = read(r,&dummy,sizeof(dummy));
    assert(sz = sizeof(dummy));
    if(2 == dummy)
    {
        const uint64_t inc = 1;
        ssize_t sz2 = write(w,&inc,sizeof(inc));
        assert(sz2 == sizeof(inc));
        return;
    }
    assert(1 == dummy);
}

int signaler_t::make_fdpair(int* r_, int* w_)
{
    int fd = eventfd(0,0);
    if(-1 == fd)
    {
        assert(errno == ENFILE || errno == EMFILE);
        *w_ = *r_ = -1;
        return -1;
    }else
    {
        *w_ = *r_ = fd;
        return 0;
    }
    
}







