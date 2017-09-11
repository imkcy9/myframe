/* 
 * File:   thread.h
 * Author: kcy
 *
 * Created on 2015年12月7日, 上午11:12
 */

#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>

typedef void (thread_fn) (void*);

class thread_t {
public:

    inline thread_t() {

    }
    void start(thread_fn* tfn_, void* arg_);
    void stop();

    thread_fn* tfn;
    void* arg;
private:
    pthread_t descriptor;

    thread_t(const thread_t&);
    const thread_t& operator=(const thread_t&);
};



#endif /* THREAD_H */

