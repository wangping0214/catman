#include <catman/net/SocketAddress.h>
#include <arpa/inet.h>

namespace catman
{
namespace net
{

SocketAddress::SocketAddress(const struct sockaddr_in &sockAddr)
{
	char cstr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &sockAddr.sin_addr, cstr, sizeof(cstr)) != NULL)
		m_addr = std::string(cstr);
	m_port = ntohs(sockAddr.sin_port);
}

SocketAddress::~SocketAddress()
{
}

std::string SocketAddress::ipAddress() const
{
	return m_addr;
}

uint16_t SocketAddress::port() const
{
	return m_port;
}

}
}

