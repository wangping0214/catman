#ifndef STREAMIO_H
#define STREAMIO_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "SessionIO.h"
 
namespace catman
{
namespace net
{

class StreamIO : public SessionIO
{
public:
	StreamIO(int fd, Session *session);
	~StreamIO();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
};

}
}

#endif
