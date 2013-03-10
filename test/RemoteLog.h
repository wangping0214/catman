#ifndef REMOTELOG_H
#define REMOTELOG_H

#include <catman/common/Protocol.h>
#include <catman/net/SessionManager.h>
#include <catman/common/LogUtil.h>
#include <log4cxx/logger.h>
#include <iostream>
#include <string>

using namespace std;

log4cxx::LoggerPtr rl_logger(log4cxx::Logger::getLogger("RemoteLog"));

class RemoteLog : public catman::common::Protocol
{
public:
	RemoteLog() : catman::common::Protocol(100)
	{
	}
	RemoteLog(const RemoteLog &other) : catman::common::Protocol(other), message(other.message)
	{
	}
	Protocol* clone() const
	{
		return new RemoteLog(*this);
	}
	virtual catman::common::OctetsStream& marshal(catman::common::OctetsStream &os) const
	{
		os << message;
		return os;
	}
	virtual const catman::common::OctetsStream& unmarshal(const catman::common::OctetsStream &os) 
	{
		os >> message;
		return os;
	}
	virtual void process(catman::net::SessionManager *manager, size_t sessionId)
	{
		catman::common::LogDebug(rl_logger, message.c_str());
	}
public:
	std::string message;
};

#endif

