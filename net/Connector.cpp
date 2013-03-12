#include <catman/net/Connector.h>
#include <catman/net/Poller.h>
#include <catman/net/Session.h>
#include <catman/net/StreamIO.h>
#include <catman/common/Configuration.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <poll.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr Connector::logger(log4cxx::Logger::getLogger("catman/net/Connector"));

Connector* Connector::open(const Session &session)
{
	const common::Configuration &conf = common::Configuration::instance();
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, conf.attributeValue("Client", "address").c_str(), &(addr.sin_addr)) != 1)
		; 	// exception
	unsigned short port = atoi(conf.attributeValue("Client", "port").c_str());
	addr.sin_port = htons(port);
	int val = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
	val = atoi(conf.attributeValue("Client", "sendbufsize").c_str());
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &val, sizeof(val));
	val = atoi(conf.attributeValue("Client", "recvbufsize").c_str());
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val));
	return (Connector*)Poller::instance().registerPollIO(new Connector(sockfd, POLLIN | POLLOUT, *((sockaddr*)(&addr)), session));
}

Connector::Connector(int fd, int initEvent, const struct sockaddr &addr, const Session &session) : PollIO(fd, initEvent), 
	m_session(session.clone())
{
	connect(m_fd, &addr, sizeof(addr));	//TODO
}

Connector::~Connector()
{
	int optVal = -1;
	socklen_t optLen = sizeof(optVal);
	int optRet = getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &optVal, &optLen);
	if (0 == optRet && 0 == optVal)
		Poller::instance().registerPollIO(new StreamIO(dup(m_fd), POLLIN, m_session)); //TODO
	else
		LOG4CXX_WARN(logger, "Failed to connect");
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

