#include "ThreadPool.h"

namespace catman
{
namespace thread
{

ThreadPool::ThreadPool(size_t maximumPoolSize) : m_maximumPoolSize(maximumPoolSize)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::execute(Runnable *runnable)
{
}

size_t ThreadPool::maximumPoolSize() const
{
	return m_maximumPoolSize;
}

}
}
