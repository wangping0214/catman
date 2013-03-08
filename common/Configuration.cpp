#include "Configuration.h"
#include "catman/tinyxml2/tinyxml2.h"

namespace catman
{
namespace common
{

Configuration Configuration::s_confInstance;

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

const Configuration& Configuration::instance(const std::string &confFile)
{
	s_confInstance.load(confFile);
	return s_confInstance;
}

const Configuration& Configuration::instance()
{
	return s_confInstance;
}

std::string Configuration::attributeValue(const std::string &section, const std::string &attrName) const
{
	ConfigurationMap::const_iterator it = m_confMap.find(section);
	if (it != m_confMap.end())
	{
		const AttributeMap &attrMap = it->second;
		AttributeMap::const_iterator attrIt = attrMap.find(attrName);
		if (attrIt != attrMap.end())
			return attrIt->second;
	}
	return std::string();
}

void Configuration::load(const std::string &confFile)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(confFile.c_str()) == 0)
	{
		tinyxml2::XMLElement *rootElem = doc.RootElement();
		for (tinyxml2::XMLElement *elem = rootElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string section(elem->Name());
			m_confMap[section];
			for (const tinyxml2::XMLAttribute *attr = elem->FirstAttribute(); attr != NULL; attr = attr->Next())
			{
				m_confMap[section][std::string(attr->Name())] = std::string(attr->Value());	
			}
		}
	}
	// else exception
}

}
}

