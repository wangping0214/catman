#ifndef CONDITION_H
#define CONDITION_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Mutex.h"
 
namespace catman
{
namespace thread
{

class Condition
{
public:
	Condition();
	~Condition();
	bool wait(Mutex *mutexPtr);
	void wakeAll();
	void wakeOne();
private:
	pthread_cond_t m_cond;
};

}
}

#endif
