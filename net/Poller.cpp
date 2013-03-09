#include <catman/net/Poller.h>
#include <catman/net/PollIO.h>
#include <catman/common/LogUtil.h>
#include <algorithm>
#include <poll.h>
#include <stdio.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr Poller::logger(log4cxx::Logger::getLogger("catman/net/Poller"));

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
	m_ioMap[pollIO->fileDescriptor()] = pollIO;
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
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
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
	for (IOMap::iterator it = m_ioMap.begin(), ie = m_ioMap.end(); it != ie; ++ it)
	{
		PollIO *pollIO = it->second;
		int fd = pollIO->fileDescriptor();
		int newEvent = pollIO->event();
		catman::common::LogDebug(logger, "POLLEVENT: sock:%d, event:%d", fd, newEvent);
		if (newEvent & POLLCLOSE)
		{
			FD_CLR(fd, &m_readSet);
			FD_CLR(fd, &m_writeSet);
			//m_ioMap[fd] = NULL; // must erase
			m_ioMap.erase(it);
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
	pollIO->detectCloseEvent();
}

}
}

