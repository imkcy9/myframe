/* 
 * File:   i_poll_events.hpp
 * Author: kcy
 *
 * Created on 2015年12月7日, 上午10:04
 */

#ifndef I_POLL_EVENTS_H
#define I_POLL_EVENTS_H

struct i_poll_events {

    virtual ~i_poll_events() {
    }

    virtual void in_event() {};

    virtual void out_event() {};

    virtual void timer_event(int id_) {};
};



#endif /* I_POLL_EVENTS_H */

