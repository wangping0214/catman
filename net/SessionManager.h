#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "common/Protocol.h"
#include "thread/ReadWriteLock.h"
#include <map>
 
namespace catman
{
namespace net
{

class Session;

class SessionManager
{
	friend class Session;
	typedef std::map<size_t, Session*> IdSessionMap;
public:
	SessionManager();
	~SessionManager();
	virtual void onAddSession(size_t sessionId) = 0;	// post event
	virtual void onDeleteSession(size_t sessionId) = 0; // pre event
	virtual void onAbortSession(size_t sessionId) = 0;
	bool send(size_t sessionId, common::Protocol *protocol);
	bool send(size_t sessionId, const common::Protocol *protocol);
	bool send(size_t sessionId, common::Protocol &protocol);
	bool send(size_t sessionId, const common::Protocol &protocol);
private:
	void addSession(size_t sessionId, Session *session);
	void deleteSession(size_t sid);
	void abortSession(size_t sid);
	Session* session(size_t sessionId);					// no lock
private:
	IdSessionMap m_idSessionMap;
	thread::ReadWriteLock m_mapRWLock;
};

}
}

#endif
