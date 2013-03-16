#ifndef RPC_H
#define RPC_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/OctetsStream.h>
#include <catman/net/Protocol.h>
#include <catman/thread/Mutex.h>
#include <catman/thread/ReadWriteLock.h>
#include <stdint.h>

namespace catman
{
namespace net
{

class RpcData : public common::Marshal
{
public:
	virtual ~RpcData();
	virtual RpcData* clone() const = 0;
	virtual void destroy();
	virtual RpcData& operator = (const RpcData &other);
};

template<typename T> class RpcArgument : public RpcData
{
public:
	RpcArgument()
	{
	}
	explicit RpcArgument(const T &data) : m_data(data)
	{
	}
	RpcArgument(const RpcArgument &other) : m_data(other.m_data)
	{
	}
	virtual ~RpcArgument()
	{
	}
	virtual RpcData* clone() const
	{
		return new RpcArgument(*this);
	}
	virtual RpcData& operator = (const RpcData &other)
	{
		const RpcArgument *argPtr = dynamic_cast<const RpcArgument*>(&other);
		if (argPtr && argPtr != this)
			m_data = argPtr->m_data;
		return *this;
	}
	virtual common::OctetsStream& marshal(common::OctetsStream &stream) const
	{
		return stream << m_data;
	}
	virtual const common::OctetsStream& unmarshal(const common::OctetsStream &stream)
	{
		return stream >> m_data;
	}
	T& data()
	{
		return m_data;
	}
private:
	T m_data;
};

template<typename T> class RpcResult : public RpcData
{
public:
	RpcResult()
	{
	}
	explicit RpcResult(int retCode, const T &data) : m_retcode(retcode), m_data(data)
	{
	}
	RpcResult(const RpcResult &other) : m_retcode(other.m_retcode), m_data(other.m_data)
	{
	}
	virtual ~RpcResult()
	{
	}
	virtual RpcData* clone() const
	{
		return new RpcResult(*this);
	}
	virtual RpcData& operator = (const RpcData &other)
	{
		const RpcResult *resPtr = dynamic_cast<const RpcResult*>(&other);
		if (resPtr && resPtr != this)
		{
			m_retcode = resPtr->m_retcode;
			m_data = resPtr->m_data;
		}
		return *this;
	}
	virtual common::OctetsStream& marshal(common::OctetsStream &stream) const
	{
		return stream << m_retcode << m_data;
	}
	virtual const common::OctetsStream& unmarshal(const common::OctetsStream &stream)
	{
		return stream >> m_retcode >> m_data;
	}
	int32_t retcode() const
	{
		return m_retcode;
	}
	T& data() 
	{
		return m_data;
	}
private:
	int32_t m_retcode;
	T m_data;

};

class RpcNoneData : public common::Marshal
{
public:
	virtual common::OctetsStream& marshal(common::OctetsStream &stream) const;
	virtual const common::OctetsStream& unmarshal(const common::OctetsStream &stream);
};

class Rpc : public Protocol
{	
private:
	class RpcId : public common::Marshal
	{
	public:
		RpcId();
		RpcId(const RpcId &other);
		RpcId& operator = (const RpcId &other);
		bool operator <(const RpcId &other) const;
		uint32_t id() const;
		void setId(uint32_t id);
		bool isRequest() const;
		void clearRequest();
		void setRequest();
		common::OctetsStream& marshal(common::OctetsStream &stream) const;
		const common::OctetsStream &unmarshal(const common::OctetsStream &stream);
	private:
		union
		{
			struct
			{
				uint32_t m_count : 31;
				uint32_t m_isRequest : 1;
			};
			uint32_t m_id;
		};
		static uint32_t s_countSeed;
		static thread::Mutex s_seedLock;
	};
	
	typedef std::map<RpcId, Rpc*> IdRpcMap;
public:
	Rpc(uint32_t type, RpcData *arg, RpcData *res); 
	Rpc(const Rpc &other);
	virtual ~Rpc();
	virtual void process(SessionManager *manager, uint32_t sessionId);
	virtual void onServer(RpcData *argument, RpcData *result, SessionManager *manager, uint32_t sessionId) = 0;
	virtual void onClient(RpcData *argument, RpcData *result, SessionManager *manager, uint32_t sessionId) = 0;
	common::OctetsStream& marshal(common::OctetsStream &stream) const;
	const common::OctetsStream& unmarshal(const common::OctetsStream &stream);
	static Rpc* call(uint32_t type);
	static Rpc* call(uint32_t type, const RpcData &arg);
protected:
	RpcId m_rpcId;
	RpcData *m_argument;
	RpcData *m_result;

	static IdRpcMap s_rpcMap;
	static thread::ReadWriteLock s_rpcMapLock;
};

}
}

#endif

