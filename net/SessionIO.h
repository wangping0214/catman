#ifndef SESSIONIO_H
#define SESSIONIO_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/net/PollIO.h>
 
namespace catman
{
namespace net
{

class Session;

class SessionIO : public PollIO
{
protected:
	SessionIO(int fd, Session *session);
protected:
	Session *m_session;
};

}
}

#endif
