#ifndef SESSION_H
#define SESSION_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "common/Octets.h"
#include "common/Protocol.h"
#include "thread/Mutex.h"

namespace catman
{
namespace net
{

class PollIO;
class StreamIO;

class Session
{
	friend class StreamIO;
public:
	Session();
	~Session();
	bool send(common::Octets buff);	// application layer	->	session layer
	bool send(common::Protocol *protocol);	// application layer 	-> 	session layer
	bool send(const common::Protocol *protocol);
	virtual void onRecv();			// transport layer 		->	session layer
	virtual void onOpen();			// transport layer		->	session layer
	virtual void onClose();
	virtual void onAbort();
private:
	common::Octets& inBuffer();
	common::Octets& outBuffer();
	thread::Mutex& outLock();
private:
	PollIO *m_pollIO;
	common::Octets m_inBuffer;	// for PollIO
	common::Octets m_outBuffer; // for PollIO
	thread::Mutex m_outLock;	// for send
};

}
}

#endif

