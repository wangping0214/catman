#include "StreamIO.h"
#include "Session.h"
#include <unistd.h>
#include <errno.h>

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
	m_session->onClose();
	m_session->destroy();
}

void StreamIO::pollIn()
{
	common::Octets& inBuffer = m_session->inBuffer();
	// TODO: handle exceptional occasion
	int recvSize;
	do
	{
		recvSize = read(m_fd, inBuffer.end(), inBuffer.capacity() - inBuffer.size());
		if (recvSize > 0)
		{
			m_session->onRecv();
			// if inBuffer is full now.
			if (inBuffer.capacity() == inBuffer.size())
				forbidRecv();
		}
	}
	while (recvSize == -1 && errno == EINTR); // was interrupted by signal, then retry.
	// recvSize == 0 means no message are available or peer has done an orderly shutdown.
	if (recvSize != -1 || errno != EAGAIN)
	{
		thread::MutexLocker locker(&m_session->outLock());
		m_session->outBuffer().clear();
		m_session->close(false);
	}
}

void StreamIO::pollOut()
{
	thread::MutexLocker locker(&(m_session->outLock()));
	common::Octets& outBuffer = m_session->outBuffer();
	// TODO: handle exceptional occasion
	int sendSize;
	do
	{
		sendSize = write(m_fd, outBuffer.begin(), outBuffer.size());
		if (sendSize > 0)
		{
			outBuffer.erase(0, sendSize);
			if (outBuffer.size() == 0)
				forbidSend();
			return;
		}
	} 
	while (sendSize == -1 && errno == EINTR); // was interrupted by signal, then retry.
	
	// Close event from application layer, guarantee all data have been sent.
	if (sendSize != 0 && errno != EAGAIN)
	{
		m_session->outBuffer().clear();
		m_session->close(false);
	}
}

void StreamIO::detectCloseEvent()
{
	thread::MutexLocker locker(&(m_session->outLock()));
	// 
	if (m_session->isClosing() && m_session->outBuffer().size() == 0)
		close();
}

}
}
