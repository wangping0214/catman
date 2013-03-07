#include "StreamIO.h"

namespace catman
{
namespace net
{

StreamIO::StreamIO(int fd, Session *session) : SessionIO(fd, s)
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
		session->onRecv();
		// if inBuffer is full now.
		if (inBuffer.capacity() == inBuffer.size())
			forbidRecv();
	}
	// recvSize == 0 means no message are available or peer has done an orderly shutdown.
}

void StreamIO::pollOut()
{
	Thread::MutexLocker locker(&(session->outLock));
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