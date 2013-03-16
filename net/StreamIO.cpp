#include <catman/net/StreamIO.h>
#include <catman/net/Session.h>
#include <catman/common/LogUtil.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr StreamIO::logger(log4cxx::Logger::getLogger("catman/net/StreamIO"));

StreamIO::StreamIO(int fd, int initEvent, Session *session) : SessionIO(fd, initEvent, session)
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
			inBuffer.resize(inBuffer.size() + recvSize);
			m_session->onRecv();
			// if inBuffer is full now.
			if (inBuffer.capacity() == inBuffer.size())
				forbidRecv();
			return;
		}
	}
	while (recvSize == -1 && errno == EINTR); // was interrupted by signal, then retry.
	common::LogDebug(logger, "RecvSize=%d, errno=%d", recvSize, errno);
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
			common::LogDebug(logger, "SendSize=%d", sendSize);
			outBuffer.erase(0, sendSize);
			if (outBuffer.size() == 0)
				forbidSend();
			return;
		}
	} 
	while (sendSize == -1 && errno == EINTR); // was interrupted by signal, then retry.
	
	// Close event from application layer, guarantee all data have been sent.
	if (sendSize != -1 || errno != EAGAIN)
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
