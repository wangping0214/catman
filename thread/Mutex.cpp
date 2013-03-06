#include "Mutex.h"

namespace catman
{
namespace thread
{

Mutex::Mutex(RecursiveType type)
{
	pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_destory(&m_mutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}

bool Mutex::tryLock()
{
	pthread_mutex_trylock(&m_mutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

MutexLocker::MutexLocker(Mutex *mutexPtr) : m_mutexPtr(mutexPtr)
{
	m_mutexPtr->lock();
}

MutexLocker::~MutexLocker()
{
	m_mutexPtr->unlock();
}

}
}
