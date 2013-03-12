#include <catman/net/SessionIO.h>
#include <catman/net/Session.h>

namespace catman
{
namespace net
{

SessionIO::SessionIO(int fd, int initEvent, Session *session) : PollIO(fd, initEvent), m_session(session)
{
	m_session->m_pollIO = this;
}

}
}
