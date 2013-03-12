#include <catman/net/PollIO.h>
#include <catman/net/Poller.h>
#include <catman/thread/Mutex.h>
#include <catman/common/LogUtil.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr PollIO::logger(log4cxx::Logger::getLogger("catman/net/PollIO"));

PollIO::PollIO(int fd, int initEvent) : m_fd(fd), m_event(0), m_cachedEvent(initEvent), m_eventDirty(true)
{
	fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
}

PollIO::~PollIO()
{
	while (::close(m_fd) == -1 && errno == EINTR)
		;
}

void PollIO::permitRecv()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	if (m_eventDirty)
		m_cachedEvent |= POLLIN;
	{
		m_cachedEvent = m_event | POLLIN;
		m_eventDirty = true;
		Poller::instance().onPollIOEventChanged(this);
		// Notify Poller the current PollIO has dirty event 
	}
}

void PollIO::permitSend()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	if (m_eventDirty)
		m_cachedEvent |= POLLOUT;
	else
	{
		m_cachedEvent = m_event | POLLOUT;
		m_eventDirty = true;
		Poller::instance().onPollIOEventChanged(this);
		// Notify Poller the current PollIO has dirty event
	}
}

void PollIO::forbidRecv()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	if (m_eventDirty)
		m_cachedEvent |= ~POLLIN;
	else
	{
		m_cachedEvent = m_event & ~POLLIN;
		m_eventDirty = true;
		Poller::instance().onPollIOEventChanged(this);
		// Notify Poller the current PollIO has dirty event
	}
}

void PollIO::forbidSend()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	if (m_eventDirty)
		m_cachedEvent |= ~POLLOUT;
	else
	{
		m_cachedEvent = m_event & ~POLLOUT;
		m_eventDirty = true;
		Poller::instance().onPollIOEventChanged(this);
		// Notify Poller the current PollIO has dirty event
	}
}

void PollIO::close()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	m_cachedEvent |= POLLCLOSE;
	if (!m_eventDirty)
		Poller::instance().onPollIOEventChanged(this);
}

int PollIO::fileDescriptor() const
{
	return m_fd;
}

int PollIO::event() const
{
	return m_event;
}

/* The synchronize operation forces PollIO and Poller
 * share the same event lock. */
void PollIO::synchronizeEvent()
{
	m_event = m_cachedEvent;
	m_cachedEvent = 0;
	m_eventDirty = false;
}

}

}


