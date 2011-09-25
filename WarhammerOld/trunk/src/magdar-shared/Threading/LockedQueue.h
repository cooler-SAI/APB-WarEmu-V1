#ifndef _THREADING_LOCKED_QUEUE_H
#define _THREADING_LOCKED_QUEUE_H

#include "Mutex.h"
#include <deque>

template<class TYPE>
class LockedQueue
{
public:
	~LockedQueue()
	{

	}

	__forceinline void add(const TYPE& element)
	{
		mutex.Acquire();
		queue.push_back(element);
		mutex.Release();
	}

	__forceinline TYPE next()
	{
		mutex.Acquire();
		assert(queue.size() > 0);
		TYPE t = queue.front();
		queue.pop_front();
		mutex.Release();
		return t;
	}

	__forceinline size_t size()
	{
		mutex.Acquire();
		size_t c = queue.size();
		mutex.Release();
		return c;
	}

	__forceinline bool empty()
	{	// return true only if sequence is empty
		mutex.Acquire();
		bool isEmpty = queue.empty();
		mutex.Release();
		return isEmpty;
	}

	__forceinline TYPE get_first_element()
	{
		mutex.Acquire();
		TYPE t; 
		if(queue.size() == 0)
			t = reinterpret_cast<TYPE>(0);
		else
			t = queue.front();
		mutex.Release();
		return t;			
	}

	__forceinline void pop()
	{
		mutex.Acquire();
		ASSERT(queue.size() > 0);
		queue.pop_front();
		mutex.Release();
	}

protected:
	std::deque<TYPE> queue;
	Mutex mutex;
};

#endif
