#include <catman/thread/Mutex.h>
#include <catman/common/LogUtil.h>

namespace catman
{
namespace thread
{

Mutex::Mutex(const std::string &identity, RecursiveType type) : m_identity(identity)
{
	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	if (type == NonRecursive)
		pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
	else
		pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}

bool Mutex::tryLock()
{
	return 0 == pthread_mutex_trylock(&m_mutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

std::string Mutex::identity() const
{
	return m_identity;
}

///////////////////////////////////////////////////////////////
log4cxx::LoggerPtr MutexLocker::logger(log4cxx::Logger::getLogger("catman/thread/MutexLocker"));

MutexLocker::MutexLocker(Mutex *mutexPtr) : m_mutexPtr(mutexPtr)
{
//	common::LogDebug(logger, "lock: %s", m_mutexPtr->identity().c_str());
	m_mutexPtr->lock();
}

MutexLocker::~MutexLocker()
{
//	common::LogDebug(logger, "unlock: %s", m_mutexPtr->identity().c_str());
	m_mutexPtr->unlock();
}

}
}
