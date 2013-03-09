#ifndef LOGUTIL_H
#define LOGUTIL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <log4cxx/logger.h>

namespace catman
{
namespace common
{

void LogDebug(log4cxx::LoggerPtr, const char *fmt, ...);

}
}

#endif

