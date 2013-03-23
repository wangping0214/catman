#ifndef TOOLKIT_H
#define TOOLKIT_H

namespace catman
{
namespace common
{

/*
 * 将str中的torep都替换成rep。
 * 
 * str 		- 需要修改的字符串
 * torep 	- 被替换的字符串
 * rep 		- 替换的字符串 
 */
void StringReplace(char *str, const char *torep, const char *rep);

}
}

#endif
