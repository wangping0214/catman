#ifndef SESSION_H
#define SESSION_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/Octets.h>
#include <catman/thread/Mutex.h>

namespace catman
{

namespace common
{
class Protocol;
}

namespace net
{

class PollIO;
class SessionIO;
class StreamIO;
class SessionManager;

class Session
{
	friend class SessionIO;
	friend class StreamIO;
public:
	Session(SessionManager *manager);
	Session(const Session& other);
	~Session();
	Session* clone() const;
	bool send(common::Octets buff);	// application layer	->	session layer
	bool send(common::Protocol *protocol);	// application layer 	-> 	session layer
	bool send(const common::Protocol *protocol);
	virtual void onRecv();			// transport layer 		->	session layer
	virtual void onOpen();			// transport layer		->	session layer
	virtual void onClose();
	virtual void onAbort();
	void close(bool needLock);
	bool isClosing() const;
	void destroy();
private:
	common::Octets& inBuffer();
	common::Octets& outBuffer();
	thread::Mutex& outLock();
	static size_t nextId();
private:
	PollIO *m_pollIO;
	common::Octets m_inBuffer;	// for PollIO
	common::Octets m_outBuffer; // for PollIO
	thread::Mutex m_outLock;	// for send
	SessionManager *m_manager;	// which manager belongs to
	size_t m_id;				// id
	bool m_closing;
	
	static size_t s_idSeed;
	static thread::Mutex s_seedLock;
};

}
}

#endif

