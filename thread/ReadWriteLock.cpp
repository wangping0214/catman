#include <catman/thread/ReadWriteLock.h>

namespace catman
{
namespace thread
{

ReadWriteLock::ReadWriteLock()
{
	pthread_rwlock_init(&m_rwlock, NULL);
}

ReadWriteLock::~ReadWriteLock()
{
	pthread_rwlock_destroy(&m_rwlock);
}

void ReadWriteLock::lockForRead()
{
	pthread_rwlock_rdlock(&m_rwlock);
}

void ReadWriteLock::lockForWrite()
{
	pthread_rwlock_wrlock(&m_rwlock);
}

bool ReadWriteLock::tryLockForRead()
{
	return 0 == pthread_rwlock_tryrdlock(&m_rwlock);
}

bool ReadWriteLock::tryLockForWrite()
{
	return 0 == pthread_rwlock_trywrlock(&m_rwlock);
}

void ReadWriteLock::unlock()
{
	pthread_rwlock_unlock(&m_rwlock);
}

ReadLocker::ReadLocker(ReadWriteLock *rwlockPtr) : m_rwlockPtr(rwlockPtr)
{
	m_rwlockPtr->lockForRead();
}

ReadLocker::~ReadLocker()
{
	m_rwlockPtr->unlock();
}

WriteLocker::WriteLocker(ReadWriteLock *rwlockPtr) : m_rwlockPtr(rwlockPtr)
{
	m_rwlockPtr->lockForWrite();
}

WriteLocker::~WriteLocker()
{
	m_rwlockPtr->unlock();
}

}
}
