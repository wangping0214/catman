#ifndef NETSESSION_H
#define NETSESSION_H

#include "common/Octets.h"

namespace catman
{
namespace net
{

class PollIO;

class NetSession
{
public:
	NetSession();
	~NetSession();
	bool send(common::Octets buff);	// application layer	->	session layer
	virtual void onRecv();	// transport layer 		->	session layer
	virtual void onOpen();	// transport layer		->	session layer
	virtual void onClose();
	virtual void onAbort();
private:
	PollIO *m_pollIO;
	common::Octets m_inBuffer;	// for PollIO
	common::Octets m_outBuffer; // for PollIO
};

}
}

#endif

