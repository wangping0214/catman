#ifndef PROCESSTASK_H
#define PROCESSTASK_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/thread/Runnable.h>
#include <catman/net/SessionManager.h>

namespace catman
{
namespace net 
{

class Protocol;

class ProcessTask : public thread::Runnable
{
public:
	ProcessTask(Protocol *protocol, net::SessionManager *manager, size_t sessionId);
	~ProcessTask();
	void run();
private:
	Protocol *m_protocol;
	net::SessionManager *m_manager;
	size_t m_sessionId;
};

}
}

#endif
