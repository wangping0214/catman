#include "RpcDataDef.h"
#include "TabString.h"
#include <assert.h>
#include <algorithm>
#include <iterator>

RpcDataDef::RpcDataDef(const tinyxml2::XMLElement *defElem) : m_name(defElem->Attribute("name"))
{
	for (const tinyxml2::XMLElement *fieldElem = defElem->FirstChildElement(); NULL != fieldElem; fieldElem = fieldElem->NextSiblingElement())
		m_fields.push_back(Field(fieldElem));
}

RpcDataDef::~RpcDataDef()
{
}

void RpcDataDef::write(const std::string &dirPath, uint32_t tabCount) const
{
	std::string filePath(dirPath);
	if (!filePath.empty())
	{
		if (filePath.find_last_of('/') != (filePath.size() - 1))
			filePath += '/';
	}
	filePath += m_name;
	FILE *destFile = fopen(filePath.c_str(), "w+");
	assert(NULL != destFile);
	std::string upperName;
	std::transform(m_name.begin(), m_name.end(), std::back_inserter(upperName), toupper);
	fprintf(destFile, "%s#ifndef %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "%s#define %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "\n");
	fprintf(destFile, "%s#include <catman/net/Rpc.h>\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	fprintf(destFile, "%sclass %s : public catman::net::RpcData\n", TabString::get(tabCount), m_name.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount));
	writeFields(destFile, tabCount);
	writeMethods(destFile, tabCount);
	fprintf(destFile, "%s};\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	fprintf(destFile, "%s#endif\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
}

void RpcDataDef::writeFields(FILE *destFile, uint32_t tabCount) const
{
	fprintf(destFile, "%spublic:\n", TabString::get(tabCount));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		it->write(destFile, tabCount + 1);
}

void RpcDataDef::writeMethods(FILE *destFile, uint32_t tabCount) const
{
	fprintf(destFile, "%spublic:\n", TabString::get(tabCount));
	// default constructor
	fprintf(destFile, "%s%s()\n", TabString::get(tabCount + 1), m_name.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// copy constructor
	std::string paramList;
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		paramList += ", " + it->name() + "(other." + it->name() + ")";
	fprintf(destFile, "%s%s(const %s &other) : catman::net::RpcData(other)%s\n", TabString::get(tabCount + 1), m_name.c_str(), m_name.c_str(), paramList.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// clone method
	fprintf(destFile, "%svirtual catman::net::RpcData* clone() const\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%sreturn new %s(*this);\n", TabString::get(tabCount + 2), m_name.c_str());
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// destroy method
	fprintf(destFile, "%svirtual void destroy()\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%sdelete this;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// operator =
	fprintf(destFile, "%svirtual RpcData& operator = (const catman::net::RpcData &other)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%sconst %s *pOther = (const %s*)(&other);\n", TabString::get(tabCount + 2), m_name.c_str(), m_name.c_str());
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		fprintf(destFile, "%s%s = pOther->%s;\n", TabString::get(tabCount + 2), it->name().c_str(), it->name().c_str());
	fprintf(destFile, "%sreturn *this;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// marshal method
	fprintf(destFile, "%svirtual catman::common::OctetsStream& marshal(catman::common::OctetsStream &stream) const\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		fprintf(destFile, "%sstream << %s;\n", TabString::get(tabCount + 2), it->name().c_str());
	fprintf(destFile, "%sreturn stream;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// unmarshal method
	fprintf(destFile, "%svirtual const catman::common::OctetsStream& unmarshal(const catman::common::OctetsStream &stream)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		fprintf(destFile, "%sstream >> %s;\n", TabString::get(tabCount + 2), it->name().c_str());
	fprintf(destFile, "%sreturn stream;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
}

