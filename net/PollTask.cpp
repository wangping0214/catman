#include <catman/net/PollTask.h>
#include <catman/net/Poller.h>
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
	Poller::instance().poll(-1);
	thread::ThreadPool::instance().execute(this);
}

}
}
