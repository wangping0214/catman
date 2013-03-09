#include <catman/net/SessionManager.h>
#include <catman/net/Session.h>

namespace catman
{
namespace net
{

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

bool SessionManager::send(size_t sessionId, common::Protocol *protocol)
{
	thread::ReadLocker locker(&m_mapRWLock);
	Session *s = session(sessionId);
	if (NULL != s)
		return s->send(protocol);
	return false;
}

bool SessionManager::send(size_t sessionId, const common::Protocol *protocol)
{
	thread::ReadLocker locker(&m_mapRWLock);
	Session *s = session(sessionId);
	if (NULL != s)
		return s->send(protocol);
	return false;
}

bool SessionManager::send(size_t sessionId, common::Protocol &protocol)
{
	return send(sessionId, &protocol);
}

bool SessionManager::send(size_t sessionId, const common::Protocol &protocol)
{
	return send(sessionId, &protocol);
}

void SessionManager::addSession(size_t sessionId, Session *session)
{
	{
		thread::WriteLocker locker(&m_mapRWLock);
		m_idSessionMap.insert(std::make_pair(sessionId, session));
	}
	onAddSession(sessionId);
}

void SessionManager::deleteSession(size_t sessionId)
{
	onDeleteSession(sessionId);
	{
		thread::WriteLocker locker(&m_mapRWLock);
		m_idSessionMap.erase(sessionId);
	}
}

void SessionManager::abortSession(size_t sessionId)
{
	onAbortSession(sessionId);
}

Session* SessionManager::session(size_t sessionId)
{
	IdSessionMap::iterator it = m_idSessionMap.find(sessionId);
	return it != m_idSessionMap.end() ? it->second : NULL;
}

}
}
