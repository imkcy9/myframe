/* 
 * File:   ypipe.h
 * Author: kcy
 *
 * Created on 2016年1月25日, 下午1:46
 */

#ifndef YPIPE_H
#define	YPIPE_H
#include "atomic_ptr.h"
#include "yqueue.h"
#include "ypipe_base.h"
#include <assert.h>

template<typename T,int N> class ypipe_t : public ypipe_base_t<T,N>
{
public:
	inline ypipe_t()
	{
		queue.push();
		r = w = f = &queue.back();
		c.set(&queue.back());
	}
	inline virtual ~ypipe_t()
	{
	
	}

	inline void write(const T &value_,bool incomplete_)
	{
		queue.back() = value_;
		queue.push() ;

		if (!incomplete_)
			f = &queue.back ();
	}

	inline bool unwrite(T *value_)
	{
		if (f == &queue.back())
		{
			return false;
		}
		queue.unpush();
		*value_ = queue.back();
		return true;
	}

	inline bool flush()
	{
		if (w == f)
		{
			return true;
		}
		if (c.cas(w,f) != w)
		{
			c.set(f);
			w = f;
			return false;
		}
		w = f;
		return true;
	}

	inline bool check_read()
	{
		if (&queue.front() != r && r)
		{
			return true;
		}

		r = c.cas(&queue.front(),NULL);
		if (&queue.front() == r || !r)
		{
			return false;
		}
		return true;
	}

	inline bool read(T *value_)
	{
		if (!check_read())
		{
			return false;
		}
		*value_ = queue.front();
		queue.pop();
		return true;
	}

	inline bool probe(bool (*fn)(T&))
	{
		bool rc = check_read();
		assert(rc);
		return (*fn)(queue.front());
	}
private:

protected:
	yqueue_t<T,N> queue;

	T *w;
	T *r;
	T *f;

	atomic_ptr_t<T> c;

	ypipe_t(const ypipe_t&);
	const ypipe_t &operator = (const ypipe_t&);
};


#endif	/* YPIPE_H */

