#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <deque>
#include <vector>

namespace catman
{
namespace thread
{

class ThreadPool
{
	typedef std::deque<Runnable*> RunnableQueue;
	typedef std::vector<ThreadPoolThread> ThreadVector; 
	class ThreadPoolThread
	{
	public:
		ThreadPoolThread();
		virtual ~ThreadPoolThread();
		virtual void run();
	};
public:
	ThreadPool(size_t maximumPoolSize);
	virtual ~ThreadPool();
	virtual void execute(Runnable *runnable);
	size_t maximumPoolSize() const;
private:
	RunnableQueue m_runnableQueue;
	ThreadVector m_activeThreads;
	size_t m_maximumPoolSize;
};

}
}

#endif
