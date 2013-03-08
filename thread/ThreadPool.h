#ifndef THREADPOOL_H
#define THREADPOOL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
//#include <deque>
#include <list>
#include <vector>
#include <stdio.h>

namespace catman
{
namespace thread
{

template<typename T> class TList
{
public:
	TList() : m_size(0) {}
	void push_front(T t)
	{
		TS[0] = t;
		incSize();
	}
	T back()
	{
		return TS[m_size - 1];
	}
	void pop_back()
	{
		decSize();
	}
	int size()
	{
		return m_size;
	}
	bool empty()
	{
		return m_size == 0;
	}
	void incSize()
	{
		printf("Inc Size\n");
		++ m_size;
	}
	void decSize()
	{
		printf("Dec Size\n");
		-- m_size;
	}
private:
	T TS[2];
	int m_size;
};

class ThreadPool
{
	class ThreadPoolThread : public Thread
	{
	public:
		ThreadPoolThread(ThreadPool *pool);
		virtual ~ThreadPoolThread();
		virtual void run();
		virtual void stop();
	private:
		ThreadPool *m_pool;
		bool m_isStopped;
	};
	typedef std::list<Runnable*> RunnableQueue;
	typedef std::vector<ThreadPoolThread*> ThreadVector; 
public:
	static ThreadPool& instance();
	explicit ThreadPool(size_t maximumPoolSize);
	virtual ~ThreadPool();
	virtual void start();
	virtual void execute(Runnable *runnable);
	size_t maximumPoolSize() const;
private:
	Runnable *fetchRunnable();
private:
	RunnableQueue m_runnableQueue;
	ThreadVector m_activeThreads;
	size_t m_maximumPoolSize;
	
	Mutex m_queueLock;
	Condition m_queueCondition;
};

}
}

#endif
