#ifndef RPCDEF_H
#define RPCDEF_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/tinyxml2/tinyxml2.h>
#include <string>
#include <stdint.h>

class RpcDef
{
public:
	RpcDef(const tinyxml2::XMLElement *rpcElem);
	~RpcDef();
	void write(const std::string &dirPath, const std::string &ns, uint32_t tabCount) const;
	std::string name() const;
	std::string argument() const;
	std::string result() const;
private:
	std::string m_name;
	uint32_t m_type;
	std::string m_argument;
	std::string m_result;
};

#endif

