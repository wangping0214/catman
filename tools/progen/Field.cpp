#include "Field.h"
#include "TabString.h"

Field::Field(const tinyxml2::XMLElement *fieldElem) : m_name(fieldElem->Attribute("name")), 
	m_type(fieldElem->Attribute("type")), m_attr(fieldElem->Attribute("attr"))
{
}

Field::~Field()
{
}

void Field::write(FILE *destFile, int tabCount) const
{
	fprintf(destFile, "%s%s %s;\n", TabString::get(tabCount), m_type.c_str(), m_name.c_str());
}

const std::string& Field::name() const
{
	return m_name;
}

const std::string& Field::type() const
{
	return m_type;
}

const std::string& Field::attr() const
{
	return m_attr;
}
