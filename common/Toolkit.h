#ifndef TOOLKIT_H
#define TOOLKIT_H

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

}
}

#endif
