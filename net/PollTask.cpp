#include "PollTask.h"
#include "Poller.h"
#include "thread/ThreadPool.h"

namespace catman
{
namespace net
{

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
	Poller::instance().poll(1000);
	thread::ThreadPool::instance().execute(this);
}

}
}
