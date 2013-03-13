#include <catman/common/Protocol.h>
#include <catman/common/OctetsStream.h>
#include <catman/common/LogUtil.h>

namespace catman
{
namespace common
{

log4cxx::LoggerPtr Protocol::logger(log4cxx::Logger::getLogger("catman/common/Protocol"));

Protocol::TypeProtocolMap Protocol::protocolStubs;

Protocol::Protocol(uint32_t type) : m_type(type)
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

uint32_t Protocol::type() const
{
	return m_type;
}

void Protocol::encode(OctetsStream &stream) const
{
//	OctetsStream data;
//	data << *this;
	stream << m_type << *this;
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
	uint32_t type = 0;
	stream >> OctetsStream::TransactionBegin;
	stream >> type;
	if ((protocol = create(type)) != NULL)
		stream >> *protocol;
	stream >> OctetsStream::TransactionCommit;
	// else consider exceptional occasion
	return protocol;
}

const Protocol* Protocol::getStub(uint32_t type)
{
	TypeProtocolMap::const_iterator it = protocolStubs.find(type);
	return it != protocolStubs.end() ? it->second : NULL;
}

Protocol* Protocol::create(uint32_t type)
{
	const Protocol* stub = getStub(type);
	return stub != NULL ? stub->clone() : NULL;
}

}
}

