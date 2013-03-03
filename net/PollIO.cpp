#include "PollIO.h"
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

namespace catman
{
namespace net
{

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
	m_event |= POLLIN;
}

void PollIO::permitSend()
{
	m_event |= POLLOUT;
}

void PollIO::forbidRecv()
{
	m_event &= ~POLLIN;
}

void PollIO::forbidSend()
{
	m_event &= ~POLLOUT;
}

void PollIO::close()
{
	m_event |= POLLCLOSE;
}

}
}


