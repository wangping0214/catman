#include "PollTask.h"
#include "Poller.h"
#include <catman/thread/ThreadPool.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr PollTask::logger(log4cxx::Logger::getLogger("catman/net/PollTask"));

PollTask::PollTask()
{
}

PollTask::~PollTask()
{
}

PollTask& PollTask::instance()
{
	static PollTask task;
	return task;
}

void PollTask::run()
{
	/*
	char msg[128];
	sprintf(msg, "Poll from thread: %lu\n", pthread_self());
	LOG4CXX_DEBUG(logger, msg);*/
	Poller::instance().poll(1000);
	thread::ThreadPool::instance().execute(this);
}

}
}
