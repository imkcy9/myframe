#include "thread.h"
#include <assert.h>

static void* thread_routine(void* arg_)
{
    thread_t* self = (thread_t*)arg_;
    self->tfn(self->arg);
    return NULL;
}

void thread_t::start(thread_fn* tfn_, void* arg_)
{
    tfn = tfn_;
    arg = arg_;
    int rc = pthread_create(&descriptor,NULL,thread_routine,this);
    assert(0 == rc);
}

void thread_t::stop()
{
    int rc = pthread_join(descriptor,NULL);
    assert(0 == rc  );
}