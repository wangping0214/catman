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
#include <locale.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <PlayerLogin.h>
#include <LoginResponse.h>
#include <Register.h>
#include <RegisterArg>
#include <RegisterRes>

using namespace std;

log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("myapp"));

PlayerLogin g_PlayerLogin;
LoginResponse g_LoginResponse;
Register g_Register(new RegisterArg, new RegisterRes);

class Server : public catman::net::SessionManager
{
public:
	virtual void onAddSession(uint32_t sessionId)
	{
		catman::common::LogDebug(g_logger, "OnAddSession: %lu", sessionId);
	}
	virtual void onDeleteSession(uint32_t sessionId)
	{
		catman::common::LogDebug(g_logger, "OnDelSession: %lu", sessionId);
	}
	virtual void onAbortSession(uint32_t sessionId)
	{
		catman::common::LogDebug(g_logger, "OnAbortSession: %lu", sessionId);
	}
};


int main(int argc, char *argv[])
{
	printf("%s\n", setlocale(LC_ALL, NULL));
	log4cxx::PatternLayout layout("%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %c %L %-5p - %m%n");
	log4cxx::ConsoleAppender appender(&layout);
	log4cxx::Logger::getRootLogger()->addAppender(&appender);
	LOG4CXX_INFO(g_logger, string("Server"));

	catman::common::Configuration::instance("catman.xml");
	Server server;
	catman::net::Acceptor::open(catman::net::Session(&server));
	catman::thread::ThreadPool &pool = catman::thread::ThreadPool::instance();
	pool.execute(&catman::net::PollTask::instance());	
	pool.start();
	return 0;
}

