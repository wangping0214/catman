#ifndef POLLTASK_H
#define POLLTASK_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/
 
#include <catman/thread/Runnable.h>
#include <log4cxx/logger.h>

namespace catman
{
namespace net
{

class PollTask : public thread::Runnable
{
public:
	~PollTask();
	static PollTask& instance();
	virtual void run();
private:
	PollTask();

	static log4cxx::LoggerPtr logger;
};

}
}

#endif
