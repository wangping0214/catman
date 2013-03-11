#ifndef MUTEX_H
#define MUTEX_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <pthread.h>
#include <log4cxx/logger.h>
#include <string>

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
//	Mutex(RecursiveType type = NonRecursive);
	Mutex(const std::string &identity);
	~Mutex();
	void lock();
	bool tryLock();
	void unlock();
	std::string identity() const;
private:
	pthread_mutex_t m_mutex;
	std::string m_identity;
};

class MutexLocker
{
public:
	MutexLocker(Mutex *mutexPtr);
	~MutexLocker();
private:
	Mutex *m_mutexPtr;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif
