#ifndef PRODEF_H
#define PRODEF_H

#include <catman/tinyxml2/tinyxml2.h>
#include "Field.h"

class ProDef
{
	typedef std::vector<Field> FieldList;
public:
	ProDef(const tinyxml2::XMLElement *proElem);
	~ProDef();
	void write(const std::string &path, int tabCount);
private:
	void writeVolatile(FILE *destFile, int tabCount);
	void writeSteady(FILE *destFile, int tabCount);
	void writeFields(FILE *destFile, int tabCount);
	void writeMethods(FILE *destFile, int tabCount);
private:
	std::string m_name;
	int	m_type;
	FieldList m_fields;
};

#endif
