#include "StreamIO.h"
#include "Session.h"
#include <unistd.h>

namespace catman
{
namespace net
{

StreamIO::StreamIO(int fd, Session *session) : SessionIO(fd, session)
{
	m_session->onOpen();
}

StreamIO::~StreamIO()
{
}

void StreamIO::pollIn()
{
	common::Octets& inBuffer = m_session->inBuffer();
	// TODO: handle exceptional occasion
	int recvSize = read(m_fd, inBuffer.end(), inBuffer.capacity() - inBuffer.size());
	if (recvSize > 0)
	{
		m_session->onRecv();
		// if inBuffer is full now.
		if (inBuffer.capacity() == inBuffer.size())
			forbidRecv();
	}
	// recvSize == 0 means no message are available or peer has done an orderly shutdown.
}

void StreamIO::pollOut()
{
	thread::MutexLocker locker(&(m_session->outLock()));
	common::Octets& outBuffer = m_session->outBuffer();
	// TODO: handle exceptional occasion
	int sendSize = write(m_fd, outBuffer.begin(), outBuffer.size());
	outBuffer.erase(0, sendSize);
	if (outBuffer.size() == 0)
		forbidSend();
	// if peer has done an orderly shutdown
}

void StreamIO::pollClose()
{
	close();
}

}
}
