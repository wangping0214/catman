#ifndef POLLER_H
#define POLLER_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/thread/Runnable.h>
#include <catman/thread/Mutex.h>
#include <catman/net/PollIO.h>
#include <log4cxx/logger.h>
#include <vector>
#include <map>
#include <set>
#include <sys/select.h>

namespace catman
{
namespace net
{

class Poller
{
	class PollControl : public PollIO
	{
	public:
		static PollControl& instance();
		virtual ~PollControl();
		void wakeup();
	private:
		PollControl(int pipeRead, int pipeWrite);
		virtual void pollIn();
		virtual void pollOut();
		virtual void detectCloseEvent();
	private:
		int m_pipeWrite;
		char m_msg[8];

		static bool s_initFlag;
	};
	typedef std::vector<int> FDSet;
	typedef std::map<int, PollIO*> IOMap;
	typedef std::set<PollIO*> IOSet;
public:
	~Poller();
	static Poller& instance();
	PollIO* registerPollIO(PollIO *pollIO);
	void poll(int timeout);
	void onPollIOEventChanged(PollIO *pollIO);
	thread::Mutex& eventLock();
private:
	Poller();
	void wakeup();
	void updateEvent();
	void triggerEvent(int fd);
private:
	FDSet m_fdSet;
	fd_set m_readSet, m_writeSet;
	int m_maxfd;
	IOMap m_ioMap;
	IOSet m_dirtyIOSet;
	bool m_canWakeup;
	thread::Mutex m_eventLock;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif

