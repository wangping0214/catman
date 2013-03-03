#include "Connector.h"
#include "Poller.h"
#include <log4cxx/logger.h>
#include <stddef.h>
#include <sys/socket.h>

namespace catman
{
namespace net
{

Connector::Connector(int fd) : PollIO(fd)
{
}

Connector::~Connector()
{
	int optVal = -1;
	socklen_t optLen = sizeof(optVal);
	int optRet = getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &optVal, &optLen);
	if (0 == optRet && 0 == optVal)
	{
		Poller::instance().registerPollIO(NULL); //TODO
	}
}

void Connector::pollIn()
{
	close();
}

void Connector::pollOut()
{
	close();
}

void Connector::pollClose()
{
}

}
}

