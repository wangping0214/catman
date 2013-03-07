#include "Poller.h"
#include "PollIO.h"
#include <algorithm>
#include <poll.h>

namespace catman
{
namespace net
{

Poller::Poller()
{
}

Poller::~Poller()
{
}

Poller& Poller::instance()
{
		static Poller poller;
		return poller;
}

PollIO* Poller::registerPollIO(PollIO *pollIO)
{
	m_ioMap[pollIO->fd()] = pollIO;
	return pollIO;
}

void Poller::poll(int timeout)
{
	updateEvent();

	int eventCount;
	if (timeout < 0)
		eventCount = ::select(m_maxfd + 1, &m_readSet, &m_writeSet, NULL, NULL);
	else
	{
		struct timeval tv;
		tv.tv_sec = timeout / timeout;
		tv.tv_usec = (timeout % timeout) * 1000;
		eventCount = ::select(m_maxfd + 1, &m_readSet, &m_writeSet, NULL, &tv);
	}
	if (eventCount > 0)
	{
		for (FDSet::const_iterator it = m_fdSet.begin(), ie = m_fdSet.end(); it != ie; ++ it)
			triggerEvent(*it);
	}
}

void Poller::updateEvent()
{
	for (IOMap::const_iterator it = m_ioMap.begin(), ie = m_ioMap.end(); it != ie; ++ it)
	{
		PollIO *pollIO = it->second;
		int fd = pollIO->fd();
		int newEvent = pollIO->event();

		if (newEvent & POLLCLOSE)
		{
			FD_CLR(fd, &m_readSet);
			FD_CLR(fd, &m_writeSet);
			m_ioMap[fd] = NULL;
			delete pollIO;
		}
		else
		{
			if (newEvent & POLLIN)
				FD_SET(fd, &m_readSet);
			else
				FD_CLR(fd, &m_readSet);

			if (newEvent & POLLOUT)
				FD_SET(fd, &m_writeSet);
			else
				FD_CLR(fd, &m_writeSet);

			m_maxfd = std::max(m_maxfd, fd);
		}
	}

	m_fdSet.clear();
	for (int i = 0; i <= m_maxfd; ++ i)
	{
		if (FD_ISSET(i, &m_readSet) || FD_ISSET(i, &m_writeSet))
			m_fdSet.push_back(i);
	}
}

void Poller::triggerEvent(int fd)
{
	PollIO *pollIO = m_ioMap[fd];
	if (FD_ISSET(fd, &m_readSet))
		pollIO->pollIn();
	if (FD_ISSET(fd, &m_writeSet))
		pollIO->pollOut();
	pollIO->pollClose();
}

///////////////////////////////////////////////////////////////

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
	//thread::ThreadPool::instance().execute(this);
}

}
}

