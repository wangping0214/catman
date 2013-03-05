#include "NetSession.h"

namespace catman
{
namespace net
{

NetSession::NetSession() : m_pollIO(NULL)
{
}

NetSession::~NetSession()
{
}

bool NetSession::send(common::Octets buff)
{
	m_outBuffer.insert(m_outBuffer.begin(), buff.begin(), buff.end());
}

void NetSession::onRecv()
{
}

void NetSession::onOpen()
{
}

void NetSession::onClose()
{
}

void NetSession::onAbort()
{
}

}
}