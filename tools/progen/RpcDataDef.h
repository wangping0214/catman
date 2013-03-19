#ifndef RPCDATADEF_H
#define RPCDATADEF_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Field.h"
#include <catman/tinyxml2/tinyxml2.h>
#include <vector>

class RpcDataDef
{
	typedef std::vector<Field> FieldList;
public:
	RpcDataDef(const tinyxml2::XMLElement *defElem);
	~RpcDataDef();
	void write(const std::string &dirPath, uint32_t tabCount) const;
private:
	void writeFields(FILE *destFile, uint32_t tabCount) const;
	void writeMethods(FILE *destFile, uint32_t tabCount) const;
private:
	std::string m_name;
	FieldList m_fields;
};

#endif

