#ifndef POLLER_H
#define POLLER_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "thread/Runnable.h"
#include <map>
#include <vector>
#include <sys/select.h>

namespace catman
{
namespace net
{

class PollIO;

class Poller
{
	typedef std::vector<int> FDSet;
	typedef std::map<int, PollIO*> IOMap;
public:
	~Poller();
	static Poller& instance();
	PollIO* registerPollIO(PollIO *pollIO);
	void poll(int timeout);
private:
	Poller();
	void updateEvent();
	void triggerEvent(int fd);
private:
	FDSet m_fdSet;
	fd_set m_readSet, m_writeSet;
	int m_maxfd;
	IOMap m_ioMap;
};

}
}

#endif

