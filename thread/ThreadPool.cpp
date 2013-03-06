#include "ThreadPool.h"
#include <unistd.h>

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
		Runnable *runnable = m_pool->fetchRunnable();
		runnable->run();
		//delete runnable;
	}
}

void ThreadPool::ThreadPoolThread::stop()
{
	m_isStopped = true;
}

////////////////////////////////////////////////////////////////

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
	MutexLocker locker(&m_queueLock);
	m_runnableQueue.push_front(runnable);
	m_queueCondition.wakeOne();
}

size_t ThreadPool::maximumPoolSize() const
{
	return m_maximumPoolSize;
}

Runnable* ThreadPool::fetchRunnable()
{
	MutexLocker locker(&m_queueLock);
	while (m_runnableQueue.empty())
		m_queueCondition.wait(&m_queueLock);
	Runnable *runnable = m_runnableQueue.back();
	m_runnableQueue.pop_back();
	return runnable;
}

}
}
