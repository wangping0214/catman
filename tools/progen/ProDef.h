#ifndef PRODEF_H
#define PRODEF_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/tinyxml2/tinyxml2.h>
#include "Field.h"
#include <vector>
#include <assert.h>

class ProDef
{
	typedef std::vector<Field> FieldList;
public:
	ProDef(const tinyxml2::XMLElement *proElem);
	~ProDef();
	void write(const std::string &path, uint32_t tabCount) const;
private:
	void writeVolatile(FILE *destFile, uint32_t tabCount) const;
	void writeSteady(FILE *destFile, uint32_t tabCount) const;
	void writeFields(FILE *destFile, uint32_t tabCount) const;
	void writeMethods(FILE *destFile, uint32_t tabCount) const;
private:
	std::string m_name;
	uint32_t m_type;
	FieldList m_fields;
};

#endif
