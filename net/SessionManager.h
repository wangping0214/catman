#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/Octets.h>
#include <catman/thread/ReadWriteLock.h>
#include <log4cxx/logger.h>
#include <map>
#include <cstdint>
 
namespace catman
{

namespace common
{
class Protocol;
}

namespace net
{

class Session;

class SessionManager
{
	friend class Session;
	typedef std::map<uint32_t, Session*> IdSessionMap;
public:
	SessionManager();
	~SessionManager();
	virtual void onAddSession(uint32_t sessionId) = 0;	// post event
	virtual void onDeleteSession(uint32_t sessionId) = 0; // pre event
	virtual void onAbortSession(uint32_t sessionId) = 0;
	bool send(uint32_t sessionId, common::Octets data);
	bool send(uint32_t sessionId, common::Protocol *protocol);
	bool send(uint32_t sessionId, const common::Protocol *protocol);
	bool send(uint32_t sessionId, common::Protocol &protocol);
	bool send(uint32_t sessionId, const common::Protocol &protocol);
private:
	void addSession(uint32_t sessionId, Session *session);
	void deleteSession(uint32_t sid);
	void abortSession(uint32_t sid);
	Session* session(uint32_t sessionId);					// no lock
private:
	IdSessionMap m_idSessionMap;
	thread::ReadWriteLock m_mapRWLock;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif
