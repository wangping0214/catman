#ifndef RPC_H
#define RPC_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/net/Protocol.h>
#include <catman/thread/Mutex.h>
#include <stdint.h>

namespace catman
{
namespace net
{

class RpcData : public common::Marshal
{
protected:
	class RpcId : public common::Marshal
	{
	public:
		RpcId();
		RpcId(const RpcId &other);
		RpcId& operator = (const RpcId &other);
		bool operator <(const RpcId &other);
		uint32_t id();
		void setId(uint32_t id);
		bool isRequest();
		void clearRequest();
		void setRequest();
		common::OctetsStream &marshal(common::OctetsStream &stream) const;
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
		static thread::Mutex s_seedMutex;
	};
public:
	virtual ~RpcData();
	virtual RpcData* clone() = 0;
	virtual void destroy();
	virtual RpcData& operator = (const RpcData &other);
};

class Rpc
{
public:
	Rpc(uint32_t type); 
private:
};

}
}

#endif
