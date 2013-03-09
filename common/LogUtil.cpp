#include <catman/common/LogUtil.h>
#include <stdarg.h>
#include <stdio.h>

namespace catman
{
namespace common
{

static const int LOGBUFSIZE = 128;

void LogDebug(log4cxx::LoggerPtr logger, const char *fmt, ...)
{
	char msg[LOGBUFSIZE];
	va_list argPtr;
	va_start(argPtr, fmt);
	vsprintf(msg, fmt, argPtr);
	va_end(argPtr);
	LOG4CXX_DEBUG(logger, msg);
}

}
}

