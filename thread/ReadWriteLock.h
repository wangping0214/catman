#ifndef READWRITELOCK_H
#define READWRITELOCK_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <pthread.h>
 
namespace catman
{
namespace thread
{

class ReadWriteLock
{
public:
	ReadWriteLock();
	~ReadWriteLock();
	void lockForRead();
	void lockForWrite();
	bool tryLockForRead();
	bool tryLockForWrite();
	void unlock();
private:
	pthread_rwlock_t m_rwlock;
};

class ReadLocker
{
public:
	ReadLocker(ReadWriteLock *rwlockPtr);
	~ReadLocker();
private:
	ReadWriteLock *m_rwlockPtr;
};

class WriteLocker
{
public:
	WriteLocker(ReadWriteLock *rwlockPtr);
	~WriteLocker();
private:
	ReadWriteLock *m_rwlockPtr;
};

}
}

#endif
