#include "Acceptor.h"
#include "Poller.h"
#include <stddef.h>
#include <sys/socket.h>

namespace catman
{
namespace net
{

Acceptor* Acceptor::open(const Session &session)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPPOTO_TCP);
	
	int val = 1;
	
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
	
	bind(sockfd, NULL, 1);
	listen(sockfd, SOMAXCONN);
	return (Acceptor*)Poller::instance().registerPollIO(new Acceptor(sockfd, session));
}

Acceptor::Acceptor(int fd, const Session &session) : PollIO(fd), m_session(session.clone())
{
}

Acceptor::~Acceptor()
{
	m_session->destroy();
}

void Acceptor::pollIn()
{
	int peerfd = accept(m_fd, NULL, NULL);
	if (-1 != peerfd)
		Poller::instance().registerPollIO(new StreamIO(peerfd, m_session->clone())); //TODO
}

void Acceptor::pollOut()
{
}

void Acceptor::detectCloseEvent()
{
}

}
}
