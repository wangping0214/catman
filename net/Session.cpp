#include "Session.h"

namespace catman
{
namespace net
{

Session::Session() : m_pollIO(NULL)
{
}

Session::~Session()
{
}

bool Session::send(common::Octets buff)
{
	m_outBuffer.insert(m_outBuffer.begin(), buff.begin(), buff.end());
	return true;
}

bool Session::send(Protocol *protocol)
{
	return false;
}

void Session::onRecv()
{
}

void Session::onOpen()
{
}

void Session::onClose()
{
}

void Session::onAbort()
{
}

common::Octets& Session::inBuffer()
{
	return m_inBuffer;
}

common::Octets& Session::outBuffer()
{
}

}
}
