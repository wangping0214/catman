#include "Connector.h"
#include "Poller.h"
#include <log4cxx/logger.h>
#include <stddef.h>
#include <sys/socket.h>

namespace catman
{
namespace net
{

Connector* Connector::open(const Session &session)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	int val = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
	
	return (Connector*)Poller::instance().registerPollIO(new Connector(sockfd, session));
}

Connector::Connector(int fd, const Session &session) : PollIO(fd), m_session(session.clone())
{
	connect(m_fd, NULL, 1);	//TODO
}

Connector::~Connector()
{
	int optVal = -1;
	socklen_t optLen = sizeof(optVal);
	int optRet = getsockopt(dup(m_fd), SOL_SOCKET, SO_ERROR, &optVal, &optLen);
	if (0 == optRet && 0 == optVal)
	{
		Poller::instance().registerPollIO(new StreamIO(m_fd, m_session)); //TODO
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

void Connector::detectCloseEvent()
{
}

}
}

