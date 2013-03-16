#ifndef RPCDEF_H
#define RPCDEF_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/tinyxml2/tinyxml2.h>
#include <string>

class RpcDef
{
public:
	RpcDef(const tinyxml2::XMLElement *rpcElem);
	~RpcDef();
	void write(const std::string &dirPath, int tabCount) const;
private:
	std::string m_name;
	int m_type;
};

#endif

