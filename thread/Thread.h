#ifndef THREAD_H
#define THREAD_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Runnable.h"
#include <pthread.h>
 
namespace catman
{
namespace thread
{

class Thread : public Runnable
{
public:
	Thread();
	~Thread();
	virtual void run();
	virtual void start();
	virtual void cancel();
private:
	pthread_t m_threadId;
};

}
}

#endif
