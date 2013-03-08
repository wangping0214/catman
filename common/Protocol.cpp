#include "Protocol.h"
#include "OctetsStream.h"

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

void Protocol::destroy()
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
	data << *this;
	stream << m_type << data;
}

Octets Protocol::encode() const
{
	OctetsStream stream;
	encode(stream);
	return (Octets)stream;
}

Octets Protocol::encode() 
{
	OctetsStream stream;
	encode(stream);
	return (Octets)stream;
}

Protocol* Protocol::decode(OctetsStream &stream)
{
	if (stream.atEnd())
		return false;
	Protocol *protocol = NULL;
	size_t type = 0;
	size_t size = 0;
	stream >> type >> size;
	if ((protocol = create(type)) != NULL)
		stream >> *protocol;
	// else consider exceptional occasion
	return protocol;
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

