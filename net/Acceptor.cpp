#include "Acceptor.h"
#include "Poller.h"
#include <stddef.h>
#include <sys/socket.h>

namespace catman
{
namespace net
{

Acceptor::Acceptor(int fd) : PollIO(fd)
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::pollIn()
{
	int peerfd = accept(m_fd, NULL, NULL);
	if (-1 != peerfd)
		Poller::instance().registerPollIO(NULL); //TODO
}

void Acceptor::pollOut()
{
}

void Acceptor::pollClose()
{
}

}
}
