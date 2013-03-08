#include "SessionIO.h"

namespace catman
{
namespace net
{

SessionIO::SessionIO(int fd, Session *session) : PollIO(fd), m_session(session)
{
	m_session->m_pollIO = this;
}

}
}
