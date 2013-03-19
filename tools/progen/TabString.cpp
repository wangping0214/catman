#include "TabString.h"
#include <string.h>

TabString::TabString()
{
}

TabString::~TabString()
{
	for (TabStringMap::iterator it = m_tabMap.begin(), ie = m_tabMap.end(); it != ie; ++ it)
		delete [](it->second);
}

TabString& TabString::instance()
{
	static TabString ts;
	return ts;
}

const char* TabString::get(uint32_t tabCount)
{
	TabStringMap &tabMap = instance().m_tabMap;
	TabStringMap::const_iterator it = tabMap.find(tabCount);
	if (it == tabMap.end())
	{
		char *tabStr = new char[tabCount + 1];
		memset(tabStr, '\t', tabCount);
		tabStr[tabCount] = '\0';
		std::pair<TabStringMap::iterator, bool> ret = tabMap.insert(std::make_pair(tabCount, tabStr));
		it = ret.first;
	}
	return it->second;
}
