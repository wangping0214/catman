#ifndef CONNECTOR_H
#define CONNECTOR_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/net/PollIO.h>
#include <log4cxx/logger.h>
#include <sys/socket.h>

namespace catman
{
namespace net
{

class Session;

class Connector : public PollIO
{
public:
	static Connector* open(const Session &session);
private:
	Connector(int fd, int initEvent, const struct sockaddr &addr, const Session &session);
	~Connector();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
private:
	Session *m_session;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif

