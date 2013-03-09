#include <catman/net/Acceptor.h>
#include <catman/net/Poller.h>
#include <catman/net/Session.h>
#include <catman/net/StreamIO.h>
#include <catman/common/Configuration.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <poll.h>

namespace catman
{
namespace net
{

Acceptor* Acceptor::open(const Session &session)
{
	const common::Configuration &conf = common::Configuration::instance();
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, conf.attributeValue("Server", "address").c_str(), &(addr.sin_addr)) != 1)
		addr.sin_addr.s_addr = INADDR_ANY;
	unsigned short port = atoi(conf.attributeValue("Server", "port").c_str());
	addr.sin_port = htons(port);
	int val = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
	val = atoi(conf.attributeValue("Server", "sendbufsize").c_str());
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &val, sizeof(val));
	val = atoi(conf.attributeValue("Server", "recvbufsize").c_str());
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val));
	bind(sockfd, (struct sockaddr*)&addr, sizeof(sockaddr_in));
	listen(sockfd, SOMAXCONN);
	return (Acceptor*)Poller::instance().registerPollIO(new Acceptor(sockfd, session));
}

Acceptor::Acceptor(int fd, const Session &session) : PollIO(fd), m_session(session.clone())
{
	m_event |= POLLIN;
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
