#ifndef ACCEPTOR_H
#define ACCEPTOR_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "PollIO.h"

namespace catman
{
namespace net
{

class Acceptor : public PollIO
{
public:
	Acceptor(int fd);
	~Acceptor();
	virtual void pollIn();
	virtual void pollOut();
	virtual void detectCloseEvent();
};

}
}

#endif
