#ifndef PROTOCOL_H
#define PROTOCOL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Octets.h"
#include "OctetsStream.h"
#include "thread/Runnable.h"
#include <map>
#include <cstddef>

namespace catman
{
namespace common
{

class net::SessionManager;

class Protocol : public Marshal
{
	typedef std::map<size_t, Protocol*> TypeProtocolMap;
public:
	virtual void process(net::SessionManager *manager, size_t sessionId) = 0;
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

