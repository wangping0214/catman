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

PollIO::PollIO(int fd) : m_fd(fd), m_event(0)
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
	m_event |= POLLIN;
	common::LogDebug(logger, "permitRecv: wakeup");
	Poller::instance().wakeup();
}

void PollIO::permitSend()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	m_event |= POLLOUT;
	common::LogDebug(logger, "permitSend: wakeup");
	Poller::instance().wakeup();
}

void PollIO::forbidRecv()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	m_event &= ~POLLIN;
	common::LogDebug(logger, "forbidRecv: wakeup");
	Poller::instance().wakeup();
}

void PollIO::forbidSend()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	m_event &= ~POLLOUT;
	common::LogDebug(logger, "forbidSend: wakeup");
	Poller::instance().wakeup();
}

void PollIO::close()
{
	thread::MutexLocker locker(&(Poller::instance().eventLock()));
	m_event |= POLLCLOSE;
	common::LogDebug(logger, "close: wakeup");
	Poller::instance().wakeup();
}

int PollIO::fileDescriptor() const
{
	return m_fd;
}

int PollIO::event() const
{
	return m_event;
}

}

}


