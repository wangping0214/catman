#include <catman/net/Rpc.h>
#include <catman/net/SessionManager.h>
#include <catman/common/OctetsStream.h>

namespace catman
{
namespace net
{

RpcData::~RpcData()
{
}

void RpcData::destroy()
{
	delete this;
}

RpcData& RpcData::operator = (const RpcData &other)
{
	return *this;
}

///////////////////////////////////////////////////////////////

uint32_t Rpc::RpcId::s_countSeed = 0;
thread::Mutex Rpc::RpcId::s_seedLock("RpcID_seedMutex");

Rpc::RpcId::RpcId() : m_id(0)
{
}

Rpc::RpcId::RpcId(const Rpc::RpcId &other) : m_id(other.m_id)
{
}

Rpc::RpcId& Rpc::RpcId::operator = (const Rpc::RpcId &other)
{
	if (&other != this)
		m_id = other.m_id;
	return *this;
}

bool Rpc::RpcId::operator < (const Rpc::RpcId &other) const
{
	return m_count < other.m_count;
}

uint32_t Rpc::RpcId::id() const
{
	return m_id;
}

void Rpc::RpcId::setId(uint32_t id)
{
	m_id = id;
}

bool Rpc::RpcId::isRequest() const
{
	return m_isRequest == 1;
}

void Rpc::RpcId::clearRequest()
{
	m_isRequest = 0;
}

void Rpc::RpcId::setRequest()
{
	m_isRequest = 1;
	thread::MutexLocker locker(&s_seedLock);
	m_count = (++ s_countSeed);
}

common::OctetsStream& Rpc::RpcId::marshal(common::OctetsStream &stream) const
{
	return stream << m_id;
}

const common::OctetsStream& Rpc::RpcId::unmarshal(const common::OctetsStream &stream)
{
	return stream >> m_id;
}

///////////////////////////////////////////////////////////////

common::OctetsStream& RpcNoneData::marshal(common::OctetsStream &stream) const
{
	return stream;
}

const common::OctetsStream& RpcNoneData::unmarshal(const common::OctetsStream &stream)
{
	return stream;
}

///////////////////////////////////////////////////////////////

Rpc::IdRpcMap Rpc::s_rpcMap;
thread::ReadWriteLock Rpc::s_rpcMapLock;

Rpc::Rpc(uint32_t type, RpcData *arg, RpcData *res) : Protocol(type), m_argument(arg), m_result(res)
{
}

Rpc::Rpc(const Rpc &other) : Protocol(other), m_rpcId(other.m_rpcId), m_argument(other.m_argument->clone()),
	m_result(other.m_result->clone())
{
}

Rpc::~Rpc()
{
	m_argument->destroy();
	m_result->destroy();
}

void Rpc::process(SessionManager *manager, uint32_t sessionId)
{
	if (m_rpcId.isRequest())
	{
		onServer(m_argument, m_result, manager, sessionId);
		m_rpcId.clearRequest();
		manager->send(sessionId, this);
	}
	else
	{
		Rpc *resRpc;
		{
			thread::ReadLocker locker(&s_rpcMapLock);		
			IdRpcMap::iterator it = s_rpcMap.find(m_rpcId);
			if (it == s_rpcMap.end())
				return;
			resRpc = it->second;
			s_rpcMap.erase(it);
		}
		resRpc->onClient(resRpc->m_argument, resRpc->m_result, manager, sessionId);
		resRpc->destroy();
	}
}

common::OctetsStream& Rpc::marshal(common::OctetsStream &stream) const
{
	return stream << m_rpcId << (m_rpcId.isRequest() ? *m_argument : *m_result);
}

const common::OctetsStream& Rpc::unmarshal(const common::OctetsStream &stream)
{
	stream >> m_rpcId;
	if (m_rpcId.isRequest())
		return stream >> *m_argument;
	else
	{
		thread::ReadLocker locker(&s_rpcMapLock);
		IdRpcMap::iterator it = s_rpcMap.find(m_rpcId);
		if (it != s_rpcMap.end())
			stream >> *(it->second->m_result);
		return stream;
	}
}

Rpc* Rpc::call(uint32_t type)
{
	return call(type, RpcArgument<RpcNoneData>());
}

Rpc* Rpc::call(uint32_t type, const RpcData &arg)
{
	Rpc *rpc = (Rpc*)Protocol::create(type);
	rpc->m_rpcId.setRequest();
	*(rpc->m_argument) = arg; 

	thread::WriteLocker locker(&s_rpcMapLock);
	IdRpcMap::iterator it = s_rpcMap.find(rpc->m_rpcId);
	if (it == s_rpcMap.end())
		s_rpcMap.insert(std::make_pair(rpc->m_rpcId, rpc));
	else
	{
		it->second->destroy();
		it->second = rpc;
		// log warning
	}
	return rpc;	
}

}
}

