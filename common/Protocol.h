#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <map>

namespace catman
{
namespace common
{

class Protocol
{
	typedef std::map<size_t, Protocol*> TypeProtocolMap;
public:
	virtual void process() = 0;
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
