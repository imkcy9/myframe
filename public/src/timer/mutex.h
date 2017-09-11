/* 
 * File:   mutex.h
 * Author: kcy
 *
 * Created on 2016年1月21日, 下午2:51
 */

#ifndef MUTEX_H
#define	MUTEX_H

#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
class mutex_t
{
public:
    inline mutex_t()
    {
        int rc = pthread_mutex_init(&mutex,NULL);
        assert(0 == rc);
    }
    
    inline ~mutex_t()
    {
        int rc = pthread_mutex_destroy(&mutex);
        assert(0 == rc);
    }
    
    inline void lock()
    {
        int rc = pthread_mutex_lock(&mutex);
        printf("%s\n",strerror(rc)); 
        assert(0 == rc);
    }
    
    inline bool try_lock()
    {
        int rc = pthread_mutex_trylock(&mutex);
        if(rc == EBUSY)
            return false;
        assert(0 == rc);
        return true;
    }
    
    inline void unlock()
    {
        int rc = pthread_mutex_unlock(&mutex);
        assert(0 == rc);
    }
    
private:
    
    pthread_mutex_t mutex;
    
    mutex_t(const mutex_t&);
    const mutex_t& operator = (const mutex_t&);
};

struct scoped_lock_t
{
    scoped_lock_t (mutex_t& mutex_)
        : mutex (mutex_)
    {
        mutex.lock ();
    }

    ~scoped_lock_t ()
    {
        mutex.unlock ();
    }

private:

    mutex_t& mutex;

    // Disable copy construction and assignment.
    scoped_lock_t (const scoped_lock_t&);
    const scoped_lock_t &operator = (const scoped_lock_t&);
};





#endif	/* MUTEX_H */

