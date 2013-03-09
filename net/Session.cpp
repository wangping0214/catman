#include <catman/net/Session.h>
#include <catman/net/PollIO.h>
#include <catman/common/OctetsStream.h>
#include <catman/common/Protocol.h>
#include <catman/common/ProcessTask.h>
#include <catman/thread/ThreadPool.h>

namespace catman
{
namespace net
{

size_t Session::s_idSeed = 0;
thread::Mutex Session::s_seedLock;

Session::Session(SessionManager *manager) : m_pollIO(NULL), m_manager(manager), m_id(nextId()), m_closing(false)
{
	m_inBuffer.reserve(8192); 	//TODO
	m_outBuffer.reserve(8192); 	//TODO
}

Session::Session(const Session &other) : m_pollIO(other.m_pollIO), m_manager(other.m_manager), m_id(nextId()), m_closing(other.m_closing) 
{
	m_inBuffer.reserve(8192); 	//TODO
	m_outBuffer.reserve(8192); 	//TODO
}

Session::~Session()
{
}

Session* Session::clone() const
{
	return new Session(*this);
}

bool Session::send(common::Octets buff)
{
	m_outBuffer.insert(m_outBuffer.begin(), buff.begin(), buff.end());
	m_pollIO->permitSend();
	return true;
}

bool Session::send(common::Protocol *protocol)
{
	return false;
}

bool Session::send(const common::Protocol *protocol)
{
	return false;
}

void Session::onRecv()
{
	common::OctetsStream stream(m_inBuffer);
	for (common::Protocol *p; (p = common::Protocol::decode(stream)) != NULL; )
	{
		common::ProcessTask *task = new common::ProcessTask(p, m_manager, m_id);
		thread::ThreadPool::instance().execute(task);
	}
}

void Session::onOpen()
{
	m_manager->addSession(m_id, this);
}

void Session::onClose()
{
	m_manager->deleteSession(m_id);
}

void Session::onAbort()
{
	m_manager->abortSession(m_id);
}

void Session::close(bool needLock)
{
	if (needLock)
	{
		thread::MutexLocker locker(&m_outLock);
		if (m_closing)
			return;
		m_closing = true;
	}
	else
	{
		if (m_closing)
			return;
		m_closing = true;
	}
	m_pollIO->permitSend();
}

bool Session::isClosing() const
{
	return m_closing;
}

void Session::destroy()
{
	delete this;
}

common::Octets& Session::inBuffer()
{
	return m_inBuffer;
}

common::Octets& Session::outBuffer()
{
	return m_outBuffer;
}

thread::Mutex& Session::outLock()
{
	return m_outLock;
}

size_t Session::nextId()
{
	thread::MutexLocker locker(&s_seedLock);
	return ++ s_idSeed;
}

}
}
