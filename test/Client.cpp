#include <catman/net/SessionManager.h>
#include <catman/common/Configuration.h>
#include <catman/common/LogUtil.h>
#include <catman/net/PollTask.h>
#include <catman/net/Connector.h>
#include <catman/net/Session.h>
#include <catman/thread/ThreadPool.h>
#include <iostream>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <catman/common/OctetsStream.h>
#include <PlayerLogin.h>
#include <LoginResponse.h>

using namespace std;

log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("myapp"));

PlayerLogin g_PlayerLogin;
LoginResponse g_LoginResponse;

class Client : public catman::net::SessionManager
{
public:
	virtual void onAddSession(size_t sessionId)
	{
		catman::common::LogDebug(g_logger, "OnAddSession: %lu", sessionId);
		PlayerLogin login;
		login.userName = "wangping";
		login.password = "admin";
		catman::common::LogDebug(g_logger, "begin to send login request");
		bool isSend = send(sessionId, login);
		if (isSend)
			catman::common::LogDebug(g_logger, "send successfully");
		else
			catman::common::LogDebug(g_logger, "send unsuccessfully");
	}
	virtual void onDeleteSession(size_t sessionId)
	{
		std::cout << "OnDelSession: " << sessionId << endl;
	}
	virtual void onAbortSession(size_t sessionId)
	{
		std::cout << "OnAbortSession: " << sessionId << endl;
	}
};


int main(int argc, char *argv[])
{
	log4cxx::PatternLayout layout("%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %c %L %-5p - %m%n");
	log4cxx::ConsoleAppender appender(&layout);
	log4cxx::Logger::getRootLogger()->addAppender(&appender);
	LOG4CXX_INFO(g_logger, string("Client"));

	catman::common::Configuration::instance("catman.xml");
	Client client;
	catman::net::Connector::open(catman::net::Session(&client));
	catman::thread::ThreadPool &pool = catman::thread::ThreadPool::instance();
	pool.execute(&catman::net::PollTask::instance());	
	pool.start();
	return 0;
}
