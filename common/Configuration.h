#ifndef CONFIGURATION_H
#define CONFIGURATION_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <map>
#include <string>

namespace catman
{
namespace common
{

class Configuration
{
	typedef std::map<std::string, std::string> AttributeMap;
	typedef std::map<std::string, AttributeMap> ConfigurationMap;
public:
	static const Configuration& instance(const std::string &confFile);
	static const Configuration& instance();
	~Configuration();
	std::string attributeValue(const std::string &section, const std::string &attrName) const;
private:
	Configuration();
	void load(const std::string &confFile);
private:
	ConfigurationMap m_confMap;	
	static Configuration s_confInstance;
};

}
}

#endif

