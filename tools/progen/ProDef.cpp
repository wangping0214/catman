#include "ProDef.h"
#include "TabString.h"
#include <algorithm>
#include <iterator>

ProDef::ProDef(const tinyxml2::XMLElement *proElem) : m_name(proElem->Attribute("name"))
{
	m_type = atoi(proElem->Attribute("type"));
	for (const tinyxml2::XMLElement *fieldElem = proElem->FirstChildElement(); NULL != fieldElem; fieldElem = fieldElem->NextSiblingElement())
		m_fields.push_back(Field(fieldElem));
}

ProDef::~ProDef()
{
}

void ProDef::write(const std::string &path, int tabCount)
{
	std::string dirPath(path);
	if (!dirPath.empty())
	{
		if (path.find_last_of('/') != (path.size() - 1)) // no '/' at end
			dirPath += '/';
	}
	// volatile part
	std::string volatileFilePath(dirPath);
	volatileFilePath += m_name;
	FILE *volatileFile = fopen(volatileFilePath.c_str(), "w+");
	assert(NULL != volatileFile);
	writeVolatile(volatileFile, tabCount);
	fclose(volatileFile);
	// steady part
	std::string steadyFilePath(dirPath);
	steadyFilePath += m_name + ".h";
	if (access(steadyFilePath.c_str(), F_OK) != 0)
	{
		FILE *steadyFile = fopen(steadyFilePath.c_str(), "w+");
		assert(NULL != steadyFile);
		writeSteady(steadyFile, 0);
	}
}

void ProDef::writeVolatile(FILE *destFile, int tabCount)
{
	writeFields(destFile, tabCount);
	writeMethods(destFile, tabCount);
}

void ProDef::writeSteady(FILE *destFile, int tabCount)
{
	std::string upperName;
	std::transform(m_name.begin(), m_name.end(), std::back_inserter(upperName), toupper);
	fprintf(destFile, "%s#ifndef %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "%s#define %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "\n");
	fprintf(destFile, "%s#include <catman/net/Protocol.h>\n", TabString::get(tabCount));
	fprintf(destFile, "%s#include <catman/common/OctetsStream.h>\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	fprintf(destFile, "%sclass %s : public catman::net::Protocol\n", TabString::get(tabCount), m_name.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount));
	fprintf(destFile, "%s#include \"%s\"\n", TabString::get(tabCount + 1), m_name.c_str());
	fprintf(destFile, "\n");
	fprintf(destFile, "%svoid process(catman::net::SessionManager *manager, uint32_t sessionId)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s};\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	fprintf(destFile, "#endif\n");
}

void ProDef::writeFields(FILE *destFile, int tabCount)
{
	fprintf(destFile, "%spublic:\n", TabString::get(tabCount));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		it->write(destFile, tabCount + 1);
}

void ProDef::writeMethods(FILE *destFile, int tabCount)
{
	fprintf(destFile, "%spublic:\n", TabString::get(tabCount));
	// default constructor
	fprintf(destFile, "%s%s() : catman::net::Protocol(%d)\n", TabString::get(tabCount + 1), m_name.c_str(), m_type);
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// copy constructor
	std::string paramList;
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		paramList += ", " + it->name() + "(" + "other." + it->name() + ")";
	fprintf(destFile, "%s%s(const %s &other) : catman::net::Protocol(other)%s\n", TabString::get(tabCount + 1), m_name.c_str(), m_name.c_str(), paramList.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// clone method
	fprintf(destFile, "%scatman::net::Protocol* clone() const\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%sreturn new %s(*this);\n", TabString::get(tabCount + 2), m_name.c_str());
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// marshal method
	fprintf(destFile, "%scatman::common::OctetsStream& marshal(catman::common::OctetsStream &os) const\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		fprintf(destFile, "%sos << %s;\n", TabString::get(tabCount + 2), it->name().c_str());
	fprintf(destFile, "%sreturn os;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// unmarshal method
	fprintf(destFile, "%sconst catman::common::OctetsStream& unmarshal(const catman::common::OctetsStream &os)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	for (FieldList::const_iterator it = m_fields.begin(), ie = m_fields.end(); it != ie; ++ it)
		fprintf(destFile, "%sos >> %s;\n", TabString::get(tabCount + 2), it->name().c_str());
	fprintf(destFile, "%sreturn os;\n", TabString::get(tabCount + 2));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
}
