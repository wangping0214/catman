#include <catman/thread/Condition.h>

namespace catman
{
namespace thread
{

Condition::Condition()
{
	pthread_cond_init(&m_cond, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&m_cond);
}

bool Condition::wait(Mutex *mutexPtr)
{
	return 0 == pthread_cond_wait(&m_cond, &(mutexPtr->m_mutex));
}

void Condition::wakeAll()
{
	pthread_cond_broadcast(&m_cond);
}

void Condition::wakeOne()
{
	pthread_cond_signal(&m_cond);
}

}
}
