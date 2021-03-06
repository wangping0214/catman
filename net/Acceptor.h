#ifndef ACCEPTOR_H
#define ACCEPTOR_H
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

class Acceptor : public PollIO
{
public:
	static Acceptor* open(const Session &session);
private:
	Acceptor(int fd, int initEvent, const Session &session);
	~Acceptor();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
private:
	Session *m_session;
};

}
}

#endif
