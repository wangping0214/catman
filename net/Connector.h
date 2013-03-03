#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "PollIO.h"

namespace catman
{
namespace net
{

class Connector : public PollIO
{
public:
	Connector(int fd);
	~Connector();
	virtual void pollIn();
	virtual void pollOut();
	virtual void pollClose();
};

}
}

#endif

