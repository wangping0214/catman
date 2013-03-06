#include "Thread.h"

namespace catman
{
namespace thread
{

static void* thread_start(void* ptr)
{
	Thread *threadPtr = (Thread*)ptr;
	threadPtr->run();
}

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::run()
{
}

void Thread::start()
{
	pthread_create(&m_threadId, NULL, thread_start, this);
}

void Thread::cancel()
{
	pthread_cancel(&m_threadId);
}

}
}