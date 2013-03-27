#include "Toolkit.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

namespace catman
{
namespace common
{

void StringReplace(char *str, const char *torep, const char *rep)
{
	assert(NULL != str);
	assert(NULL != torep);
	assert(NULL != rep);

	size_t torepLen = strlen(torep);
	size_t repLen = strlen(rep);
	size_t resultLen = strlen(str) * repLen / torepLen;
	char *result = (char*)malloc(resultLen + 1);
	memset(result, 0, resultLen);

	char *prev = str;
	char *cur = str;
	while (NULL != cur)
	{
		cur = strstr(cur, torep);
		if (NULL == cur)
			break;
		strncat(result, prev, cur - prev);
		strcat(result, rep);
		prev = cur = cur + torepLen;
	}
	strcat(result, prev);
	strcpy(str, result);
	free(result);
}

bool MakePath(const std::string &dirPath, mode_t mode)
{
	if (dirPath.empty())
		return false;
	std::string tmp(dirPath);
	if (tmp.find_last_of('/') != (tmp.size() - 1))
		tmp += '/';
	struct stat dirStat;
	// skip root path
	for (std::string::size_type i = tmp.find_first_of('/', 1); i != std::string::npos; i = tmp.find_first_of('/', i + 1))
	{
		const std::string &subdir = tmp.substr(0, i);
		if (stat(subdir.c_str(), &dirStat) != 0)
		{
			if (mkdir(subdir.c_str(), mode) != 0)
				return false;
		}
		else if (!S_ISDIR(dirStat.st_mode))
			return false;
	}
	return true;
}

}
}


