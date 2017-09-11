/* 
 * File:   signaler.h
 * Author: kcy
 *
 * Created on 2016年1月21日, 上午10:43
 */

#ifndef SIGNALER_H
#define	SIGNALER_H

#include <unistd.h>

class signaler_t
{
public:
    signaler_t();
    ~signaler_t();
    
    int get_fd();
    void send();
    int wait(int timeout_);
    void recv();
private:
    
    static int make_fdpair(int* r_, int* w);
    
    int w;
    int r;
    
    signaler_t(const signaler_t&);
    const signaler_t& operator = (const signaler_t&);
};


#endif	/* SIGNALER_H */

