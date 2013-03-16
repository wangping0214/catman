#include <catman/net/Session.h>
#include <catman/net/PollIO.h>
#include <catman/net/Protocol.h>
#include <catman/net/ProcessTask.h>
#include <catman/common/OctetsStream.h>
#include <catman/common/LogUtil.h>
#include <catman/thread/ThreadPool.h>
#include <iostream>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr Session::logger(log4cxx::Logger::getLogger("catman/net/Session"));

uint32_t Session::s_idSeed = 0;
thread::Mutex Session::s_seedLock("Session_seedLock");

Session::Session(SessionManager *manager) : m_pollIO(NULL), m_outLock("Session_outLock"), m_manager(manager), m_id(nextId()), m_closing(false) 
{
	m_inBuffer.reserve(8192); 	//TODO
	m_outBuffer.reserve(8192); 	//TODO
}

Session::Session(const Session &other) : m_pollIO(other.m_pollIO), m_outLock("Session_outLock"), m_manager(other.m_manager), m_id(nextId()), m_closing(other.m_closing)
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

// TODO must be locked by outlock, because Poll thread and other
// will change out buffer simultaneously.
bool Session::send(common::Octets buff)
{
	thread::MutexLocker locker(&m_outLock);
	m_outBuffer.insert(m_outBuffer.end(), buff.begin(), buff.end());
	m_pollIO->permitSend();
	return true;
}

bool Session::send(Protocol *protocol)
{
	return send(protocol->encode());
}

bool Session::send(const Protocol *protocol)
{
	return send(protocol->encode());
}

void Session::onRecv()
{
	common::OctetsStream stream(m_inBuffer); 	// copy
	m_inBuffer.clear(); 						// clear
	for (Protocol *p; (p = Protocol::decode(stream)) != NULL; )
	{
		ProcessTask *task = new ProcessTask(p, m_manager, m_id);
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

uint32_t Session::nextId()
{
	thread::MutexLocker locker(&s_seedLock);
	return ++ s_idSeed;
}

}
}
