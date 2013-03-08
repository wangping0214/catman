#include "ThreadPool.h"
#include <unistd.h>
#include <log4cxx/logger.h>
#include <stdio.h>

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("catman/thread/ThreadPool"));

namespace catman
{
namespace thread
{

ThreadPool::ThreadPoolThread::ThreadPoolThread(ThreadPool *pool) : m_pool(pool), m_isStopped(false)
{
}

ThreadPool::ThreadPoolThread::~ThreadPoolThread()
{
}

void ThreadPool::ThreadPoolThread::run()
{
	while (!m_isStopped)
	{
		LOG4CXX_DEBUG(logger, "run");
		Runnable *runnable = m_pool->fetchRunnable();
		runnable->run();
	}
}

void ThreadPool::ThreadPoolThread::stop()
{
	m_isStopped = true;
}

////////////////////////////////////////////////////////////////

ThreadPool& ThreadPool::instance()
{
	// must be configurable
	static ThreadPool pool(1);
	return pool;
}

ThreadPool::ThreadPool(size_t maximumPoolSize) : m_maximumPoolSize(maximumPoolSize)
{
}

ThreadPool::~ThreadPool()
{
	for (ThreadVector::iterator it = m_activeThreads.begin(), ie = m_activeThreads.end(); it != ie; ++ it)
	{
		(*it)->stop();
		delete *it;
	}
	// m_queueCondition.wakeAll();
	// pthread_join all active threads
	
}

void ThreadPool::start()
{
	for (size_t i = 0; i < m_maximumPoolSize; ++ i)
	{
		ThreadPoolThread *thread = new ThreadPoolThread(this);
		m_activeThreads.push_back(thread);
		thread->start();
	}

	// temporary
	while (true)
		sleep(1);
}

void ThreadPool::execute(Runnable *runnable)
{
//	MutexLocker locker(&m_queueLock);
	m_queueLock.lock();
	printf("execute_last %d\n", (int)m_runnableQueue.size());
	LOG4CXX_DEBUG(logger, "execute");
	m_runnableQueue.push_front(runnable);
	LOG4CXX_DEBUG(logger, "after push");

	if (m_runnableQueue.empty())
	{
		LOG4CXX_DEBUG(logger, "Empty");
	}
	else
		LOG4CXX_DEBUG(logger, "Not empty");
	printf("execute_last %d\n", (int)m_runnableQueue.size());
	m_queueCondition.wakeOne();
	m_queueLock.unlock();
}

size_t ThreadPool::maximumPoolSize() const
{
	return m_maximumPoolSize;
}

Runnable* ThreadPool::fetchRunnable()
{
//	MutexLocker locker(&m_queueLock);
	m_queueLock.lock();
	printf("fetch_begin %d\n", (int)m_runnableQueue.size());
	LOG4CXX_DEBUG(logger, "pre wait");
	if (m_runnableQueue.empty())
		LOG4CXX_DEBUG(logger, "Empty");
	while (m_runnableQueue.empty())
	//if (m_runnableQueue.empty())
	{
		LOG4CXX_DEBUG(logger, "wait");
		m_queueCondition.wait(&m_queueLock);
	}
	LOG4CXX_DEBUG(logger, "fetch");
	Runnable *runnable = m_runnableQueue.back();
	m_runnableQueue.pop_back();
	printf("fetch_last %d\n", (int)m_runnableQueue.size());
	m_queueLock.unlock();
	return runnable;
}

}
}
