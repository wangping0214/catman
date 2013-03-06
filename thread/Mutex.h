#ifndef MUTEX_H
#define MUTEX_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <pthread.h>

namespace catman
{
namespace thread
{

class Condition;

class Mutex
{
	friend class Condition;
public:
	enum RecursiveType
	{
		Recursive,
		NonRecursive
	};
	Mutex(RecursiveType type = NonRecursive);
	~Mutex();
	void lock();
	bool tryLock();
	void unlock();
private:
	pthread_mutex_t m_mutex;
};

class MutexLocker
{
public:
	MutexLocker(Mutex *mutexPtr);
	~MutexLocker();
private:
	Mutex m_mutexPtr;
};

}
}

#endif
