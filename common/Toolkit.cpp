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

}
}


