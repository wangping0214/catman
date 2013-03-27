#ifndef TOOLKIT_H
#define TOOLKIT_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <string>
#include <sys/stat.h>

namespace catman
{
namespace common
{

/* Replace all occurence of torep in str with rep.
 * 
 * str 		- the dest str.
 * torep 	- the substr to be replaced.
 * rep 		- the substr to replace.
 */
void StringReplace(char *str, const char *torep, const char *rep);

/* Creates the directory path dirPath.
 * The function will create all parent directories necessary to create the directory.
 * Returns true if successful; otherwise returns false.
 * If the path already exists when this function is called, it will return true. */
bool MakePath(const std::string &dirPath, mode_t mode);

}
}

#endif
