#ifndef THREADPOOL_H
#define THREADPOOL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include <log4cxx/logger.h>
#include <deque>
#include <vector>
#include <stdio.h>

namespace catman
{
namespace thread
{

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
	typedef std::deque<Runnable*> RunnableQueue;
	typedef std::vector<ThreadPoolThread*> ThreadVector; 
public:
	static ThreadPool& instance();
	virtual ~ThreadPool();
	virtual void start();
	virtual void execute(Runnable *runnable);
	size_t maximumPoolSize() const;
private:
	explicit ThreadPool(size_t maximumPoolSize);
	Runnable *fetchRunnable();
private:
	RunnableQueue m_runnableQueue;
	ThreadVector m_activeThreads;
	size_t m_maximumPoolSize;
	
	Mutex m_queueLock;
	Condition m_queueCondition;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif
