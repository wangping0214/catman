#include "Protocol.h"

namespace catman
{
namespace common
{

Protocol::TypeProtocolMap Protocol::protocolStubs;

Protocol::Protocol(size_t type) : m_type(type)
{
	if (getStub(type) == NULL)
		protocolStubs.insert(std::make_pair(m_type, this));
}

Protocol::Protocol(const Protocol& other) : m_type(other.m_type)
{
}

Protocol::~Protocol()
{
}

void Protocol::destory()
{
	delete this;
}

size_t Protocol::type() const
{
	return m_type;
}

void Protocol::encode(OctetsStream &stream) const
{
	OctetsStream data;
	//data << *this;
	//stream << m_type << data;
}

Octets Protocol::encode() const
{
	return Octets();
}

Octets Protocol::encode() 
{
	return Octets();
}

Protocol* Protocol::decode(OctetsStream &stream)
{
	return NULL;
}

const Protocol* Protocol::getStub(size_t type)
{
	TypeProtocolMap::const_iterator it = protocolStubs.find(type);
	return it != protocolStubs.end() ? it->second : NULL;
}

Protocol* Protocol::create(size_t type)
{
	const Protocol* stub = getStub(type);
	return stub != NULL ? stub->clone() : NULL;
}

}
}

