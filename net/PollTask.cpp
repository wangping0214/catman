#include "PollTask.h"
#include "Poller.h"
#include <catman/thread/ThreadPool.h>

#include <iostream>
using namespace std;

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
	//Poller::instance().poll(1000);
	sleep(5);
	cout << "PollTask" << endl;
	thread::ThreadPool::instance().execute(/*this*/new PollTask());
}

}
}
