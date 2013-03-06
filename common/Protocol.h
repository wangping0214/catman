#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Octets.h"
#include <map>
#include <cstddef>

namespace catman
{
namespace common
{

class Protocol
{
	typedef std::map<size_t, Protocol*> TypeProtocolMap;
public:
	virtual void process() = 0;
	virtual Protocol* clone() const = 0;
	virtual void destory();
	size_t type() const;
	void encode(OctetsStream &stream) const;
	Octets encode() const;
	Octets encode();
	static Protocol *decode(OctetsStream &stream);
	static const Protocol* getStub(size_t type);
	static Protocol* create(size_t type);
protected:
	Protocol(size_t type);
	Protocol(const Protocol &other);
	virtual ~Protocol();
protected:
	size_t m_type;
private:
	static TypeProtocolMap protocolStubs;
};

}
}

#endif

