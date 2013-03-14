#ifndef TABSTRING_H
#define TABSTRING_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <map>
#include <stddef.h>

class TabString
{
	typedef std::map<size_t, const char*> TabStringMap;
public:
	~TabString();
	static const char* get(size_t tabCount);
private:
	TabString();
	static TabString& instance();
private:
	TabStringMap m_tabMap;
};

#endif
