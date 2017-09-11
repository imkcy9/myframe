/* 
 * File:   ypipe_base.h
 * Author: kcy
 *
 * Created on 2016年1月25日, 下午1:46
 */

#ifndef YPIPE_BASE_H
#define	YPIPE_BASE_H
template <typename T, int N> class ypipe_base_t
{
public:
    virtual ~ypipe_base_t(){}
    virtual void write(const T &value_,bool incomplete_) = 0;
    virtual bool unwrite(T *value_) = 0;
    virtual bool flush() = 0;
    virtual bool check_read() = 0;
    virtual bool read(T *value_) = 0;
    virtual bool probe(bool (*fn)(T&)) = 0;
};



#endif	/* YPIPE_BASE_H */

