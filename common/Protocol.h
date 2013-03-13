#ifndef PROTOCOL_H
#define PROTOCOL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/Marshal.h>
#include <catman/common/Octets.h>
#include <log4cxx/logger.h>
#include <map>
#include <cstddef>
#include <stdint.h>

namespace catman
{

/* net namespace forward declaration */
namespace net
{
class SessionManager;
}

namespace common
{

class OctetsStream;

class Protocol : public Marshal
{
	typedef std::map<uint32_t, Protocol*> TypeProtocolMap;
public:
	virtual void process(net::SessionManager *manager, uint32_t sessionId) = 0;
	virtual Protocol* clone() const = 0;
	virtual void destroy();
	uint32_t type() const;
	void encode(OctetsStream &stream) const;
	Octets encode() const;
	Octets encode();
	static Protocol *decode(OctetsStream &stream);
	static const Protocol* getStub(uint32_t type);
	static Protocol* create(uint32_t type);
protected:
	Protocol(uint32_t type);
	Protocol(const Protocol &other);
	virtual ~Protocol();
protected:
	uint32_t m_type;
private:
	static TypeProtocolMap protocolStubs;

	static log4cxx::LoggerPtr logger;
};

}
}

#endif

