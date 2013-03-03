#ifndef ACCEPTOR_H
#define ACCEPTOR_H

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
	virtual void pollClose();
};

}
}

#endif
