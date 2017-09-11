/* 
 * File:   mailbox.h
 * Author: kcy
 *
 * Created on 2016年1月21日, 下午2:41
 */

#ifndef MAILBOX_H
#define MAILBOX_H

#include <stddef.h>
#include "signaler.h"
#include "mutex.h"
#include "ypipe.h"

template<typename T>
class mailbox_t {
public:

    mailbox_t() {
        bool ok = cpipe.read(NULL);
        assert(!ok);
        active = false;
    }

    ~mailbox_t() {
        sync.lock();
        sync.unlock();
    }

    int get_fd() {
        return signaler.get_fd();
    }

    void send(T& t) {
        sync.lock();
        cpipe.write(t, false);
        bool ok = cpipe.flush();
        sync.unlock();
        if (!ok)
            signaler.send();
    }

    int recv(T *t, int timeout_) {
        if (active) {
            bool ok = cpipe.read(t);
            if (ok)
                return 0;
            active = false;
            signaler.recv();
        }

        int rc = signaler.wait(timeout_);
        if (rc != 0 && (errno == EAGAIN || errno == EINTR))
            return -1;

        active = true;

        assert(rc == 0);
        bool ok = cpipe.read(t);
        assert(ok);
        return 0;
    }


private:
    typedef ypipe_t<T, 10> cpipe_t;
    cpipe_t cpipe;
    signaler_t signaler;
    mutex_t sync;
    bool active;
};

#endif /* MAILBOX_H */

