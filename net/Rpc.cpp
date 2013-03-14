#include <catman/net/Rpc.h>

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



}
}