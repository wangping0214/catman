#ifndef POLLIO_H
#define POLLIO_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <log4cxx/logger.h>

namespace catman
{
namespace net
{

enum
{
	POLLCLOSE = 0x80000000
};

class PollIO
{
public:
	virtual ~PollIO();
	/* trigger by POLLIN event from socket */
	virtual void pollIn() = 0;
	/* trigger by POLLOUT event from sockt */
	virtual void pollOut() = 0;
	/* Detect close event from Session or POLLIN and POLLOUT */
	virtual void detectCloseEvent() = 0;
	void permitRecv();
	void permitSend();
	void forbidRecv();
	void forbidSend();
	void close();
	int fileDescriptor() const;
	int event() const;
	/* It's not thread-safe, the caller must acquire lock outside. */
	void synchronizeEvent();
protected:
	PollIO(int fd, int initEvent);
protected:
	int m_fd;
private:
	int m_event;
	int m_cachedEvent;
	bool m_eventDirty;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif

