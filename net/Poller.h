#ifndef POLLER_H
#define POLLER_H

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
	~Poller() {}
	static Poller& instance()
	{
		static Poller poller;
		return poller;
	}
	PollIO* registerPollIO(PollIO *pollIO);
	void poll(int timeout);
private:
	Poller() {}

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

