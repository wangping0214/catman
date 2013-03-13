#include <catman/net/SessionManager.h>
#include <catman/net/Session.h>
#include <catman/common/LogUtil.h>
#include <assert.h>

namespace catman
{
namespace net
{

log4cxx::LoggerPtr SessionManager::logger(log4cxx::Logger::getLogger("catman/net/SessionManager"));

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

bool SessionManager::send(uint32_t sessionId, common::Octets data)
{
	thread::ReadLocker locker(&m_mapRWLock);
	Session *s = session(sessionId);
	if (NULL != s)
		return s->send(data);
	return false;
}

bool SessionManager::send(uint32_t sessionId, common::Protocol *protocol)
{
	thread::ReadLocker locker(&m_mapRWLock);
	Session *s = session(sessionId);
	if (NULL != s)
		return s->send(protocol);
	else
		common::LogDebug(logger, "session %lu is NULL", sessionId);
	return false;
}

bool SessionManager::send(uint32_t sessionId, const common::Protocol *protocol)
{
	thread::ReadLocker locker(&m_mapRWLock);
	Session *s = session(sessionId);
	if (NULL != s)
		return s->send(protocol);
	return false;
}

bool SessionManager::send(uint32_t sessionId, common::Protocol &protocol)
{
	return send(sessionId, &protocol);
}

bool SessionManager::send(uint32_t sessionId, const common::Protocol &protocol)
{
	return send(sessionId, &protocol);
}

void SessionManager::addSession(uint32_t sessionId, Session *session)
{
	{
		thread::WriteLocker locker(&m_mapRWLock);
		m_idSessionMap.insert(std::make_pair(sessionId, session));
	}
	onAddSession(sessionId);
}

void SessionManager::deleteSession(uint32_t sessionId)
{
	onDeleteSession(sessionId);
	{
		thread::WriteLocker locker(&m_mapRWLock);
		m_idSessionMap.erase(sessionId);
	}
}

void SessionManager::abortSession(uint32_t sessionId)
{
	onAbortSession(sessionId);
}

Session* SessionManager::session(uint32_t sessionId)
{
	IdSessionMap::iterator it = m_idSessionMap.find(sessionId);
	return it != m_idSessionMap.end() ? it->second : NULL;
}

}
}
