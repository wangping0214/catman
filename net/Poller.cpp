#include <catman/net/Poller.h>
#include <catman/net/PollIO.h>
#include <catman/common/LogUtil.h>
#include <algorithm>
#include <poll.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr Poller::logger(log4cxx::Logger::getLogger("catman/net/Poller"));

bool Poller::PollControl::s_initFlag = false;

Poller::PollControl& Poller::PollControl::instance()
{
	static int pipefds[2];
	if (!s_initFlag)
	{
		::pipe(pipefds);
		s_initFlag = true;
	}
	static PollControl pollControl(pipefds[0], pipefds[1]);
	return pollControl;
}

Poller::PollControl::PollControl(int pipeRead, int pipeWrite) : PollIO(pipeRead), m_pipeWrite(pipeWrite)
{
	fcntl(m_pipeWrite, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
	m_event |= POLLIN;
}

Poller::PollControl::~PollControl()
{
	while (::close(m_pipeWrite) == -1 && errno == EINTR)
		;
}

void Poller::PollControl::wakeup()
{
	pollOut();
}

void Poller::PollControl::pollIn()
{
	::read(m_fd, m_msg, sizeof(m_msg));
}

void Poller::PollControl::pollOut()
{
	::write(m_pipeWrite, m_msg, sizeof(m_msg));
}

void Poller::PollControl::detectCloseEvent()
{
}

///////////////////////////////////////////////////////

Poller::Poller() : m_maxfd(0), m_eventLock("Poller_eventLock")
{
	FD_ZERO(&m_readSet);
	FD_ZERO(&m_writeSet);
	//TODO add PollControl
	registerPollIO(&PollControl::instance());
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
	// It's a good time to wakeup, then select will return immediately
	m_canWakeup = true;
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
	// select has returned, wakeup is unnecessary
	m_canWakeup = false;
	if (eventCount > 0)
	{
		for (FDSet::const_iterator it = m_fdSet.begin(), ie = m_fdSet.end(); it != ie; ++ it)
			triggerEvent(*it);
	}
}

void Poller::onPollIOEventChanged(PollIO *pollIO)
{
	m_dirtyIOSet.insert(pollIO);
	wakeup();
}

thread::Mutex& Poller::eventLock() 
{
	return m_eventLock;
}

void Poller::wakeup()
{
	if (m_canWakeup)
	{
		m_canWakeup = false;
		PollControl::instance().wakeup();
	}
}

void Poller::updateEvent()
{
	/* When Poller is fetching event from PollIOs,
	 * all PollIOs are forbidden to update their
	 * events. */
	thread::MutexLocker locker(&m_eventLock);

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
			// TODO deadlock arises
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
	assert(pollIO != NULL);
	if (FD_ISSET(fd, &m_readSet))
		pollIO->pollIn();
	if (FD_ISSET(fd, &m_writeSet))
		pollIO->pollOut();
	pollIO->detectCloseEvent();
}

}
}

