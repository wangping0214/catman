#ifndef CONNECTOR_H
#define CONNECTOR_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "PollIO.h"

namespace catman
{
namespace net
{

class Connector : public PollIO
{
public:
	static Connector* open(const Session *session);
private:
	Connector(int fd);
	~Connector();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
private:
	Session *m_session;
};

}
}

#endif

