#include <catman/net/ProcessTask.h>
#include <catman/net/Protocol.h>

namespace catman
{
namespace net
{

ProcessTask::ProcessTask(Protocol *protocol, net::SessionManager *manager, size_t sessionId) : m_protocol(protocol), m_manager(manager), m_sessionId(sessionId)
{
}

ProcessTask::~ProcessTask()
{
	m_protocol->destroy();
}

void ProcessTask::run()
{
	m_protocol->process(m_manager, m_sessionId);
	delete this;
}

}
}
