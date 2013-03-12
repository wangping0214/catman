#ifndef STREAMIO_H
#define STREAMIO_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/net/SessionIO.h>
#include <log4cxx/logger.h>
 
namespace catman
{
namespace net
{

class StreamIO : public SessionIO
{
public:
	StreamIO(int fd, int initEvent, Session *session);
	~StreamIO();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
private:
	static log4cxx::LoggerPtr logger;
};

}
}

#endif
