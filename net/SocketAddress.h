#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <netinet/in.h>
#include <stdint.h>
#include <string>

namespace catman
{
namespace net
{

class SocketAddress
{
public:
	SocketAddress(const struct sockaddr_in &sockAddr);
	~SocketAddress();
	std::string ipAddress() const;
	uint16_t port() const;
private:
	std::string m_addr;
	uint16_t m_port;
};

}
}

#endif

