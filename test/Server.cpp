#include <catman/net/SessionManager.h>
#include <catman/common/Configuration.h>
#include <catman/net/PollTask.h>
#include <catman/net/Acceptor.h>
#include <catman/net/Session.h>
#include <catman/thread/ThreadPool.h>
#include <catman/common/Octets.h>
#include <catman/common/OctetsStream.h>
#include <catman/common/LogUtil.h>
#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include "RemoteLog.h"

using namespace std;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("myapp"));

class Server : public catman::net::SessionManager
{
public:
	virtual void onAddSession(size_t sessionId)
	{
		catman::common::LogDebug(logger, "OnAddSession: %lu", sessionId);
		std::string str("Message from Server");
		RemoteLog rl;
		rl.message = str;
//		send(sessionId, (catman::common::Octets)stream);
		send(sessionId, rl);
	}
	virtual void onDeleteSession(size_t sessionId)
	{
		std::cout << "OnDelSession: " << sessionId << endl;
	}
	virtual void onAbortSession(size_t sessionId)
	{
		catman::common::LogDebug(logger, "OnAbortSession: %lu", sessionId);
	}
};


int main(int argc, char *argv[])
{
//	log4cxx::BasicConfigurator::configure();
	log4cxx::PatternLayout layout("%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %c %L %-5p - %m%n");
	log4cxx::ConsoleAppender appender(&layout);
	log4cxx::Logger::getRootLogger()->addAppender(&appender);
	LOG4CXX_INFO(logger, string("Server"));

	catman::common::Configuration::instance("catman.xml");
	Server server;
	catman::net::Acceptor::open(catman::net::Session(&server));
	catman::thread::ThreadPool &pool = catman::thread::ThreadPool::instance();
	pool.execute(&catman::net::PollTask::instance());	
	pool.start();
	return 0;
}

