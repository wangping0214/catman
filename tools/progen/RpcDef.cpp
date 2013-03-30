#include "RpcDef.h"
#include "TabString.h"
#include <algorithm>
#include <iterator>

RpcDef::RpcDef(const tinyxml2::XMLElement *rpcElem) : m_name(rpcElem->Attribute("name")), m_type(atoi(rpcElem->Attribute("type"))),
	m_argument(rpcElem->Attribute("argument")), m_result(rpcElem->Attribute("result"))
{	
}

RpcDef::~RpcDef()
{
}

void RpcDef::write(const std::string &dirPath, const std::string &ns, uint32_t tabCount) const
{
	std::string filePath(dirPath);
	if (!filePath.empty())
	{
		if (filePath.find_last_of('/') != (filePath.size() - 1))
			filePath += '/';
	}
	filePath += m_name + ".h";
	if (access(filePath.c_str(), F_OK) == 0)
		return;
	FILE *destFile = fopen(filePath.c_str(), "w+");
	std::string upperName;
	std::transform(m_name.begin(), m_name.end(), std::back_inserter(upperName), toupper);
	fprintf(destFile, "%s#ifndef %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "%s#define %s_H\n", TabString::get(tabCount), upperName.c_str());
	fprintf(destFile, "\n");
	fprintf(destFile, "%s#include \"%s\"\n", TabString::get(tabCount), m_argument.c_str());
	fprintf(destFile, "%s#include \"%s\"\n", TabString::get(tabCount), m_result.c_str());
	fprintf(destFile, "%s#include <catman/net/Rpc.h>\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	if (!ns.empty())
	{
		fprintf(destFile, "%snamespace %s\n", TabString::get(tabCount), ns.c_str());
		fprintf(destFile, "%s{\n", TabString::get(tabCount));
		fprintf(destFile, "\n");
	}
	fprintf(destFile, "%sclass %s : public catman::net::Rpc\n", TabString::get(tabCount), m_name.c_str());
	fprintf(destFile, "%s{\n", TabString::get(tabCount));
	fprintf(destFile, "%spublic:\n", TabString::get(tabCount));
	// constructor
	fprintf(destFile, "%s%s(catman::net::RpcData *arg, catman::net::RpcData *res) : catman::net::Rpc(%d, arg, res)\n", TabString::get(tabCount + 1), m_name.c_str(), m_type);
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// clone
	fprintf(destFile, "%scatman::net::Protocol* clone() const\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%sreturn new %s(*this);\n", TabString::get(tabCount + 2), m_name.c_str());
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// onServer
	fprintf(destFile, "%svirtual void onServer(catman::net::RpcData *argument, catman::net::RpcData *result, catman::net::SessionManager *manager, uint32_t sessionId)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	// onClient
	fprintf(destFile, "%svirtual void onClient(catman::net::RpcData *argument, catman::net::RpcData *result, catman::net::SessionManager *manager, uint32_t sessionId)\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s{\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s}\n", TabString::get(tabCount + 1));
	fprintf(destFile, "%s};\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
	if (!ns.empty())
	{
		fprintf(destFile, "%s}\n", TabString::get(tabCount));
		fprintf(destFile, "\n");
	}
	fprintf(destFile, "%s#endif\n", TabString::get(tabCount));
	fprintf(destFile, "\n");
}

std::string RpcDef::name() const
{
	return m_name;
}

std::string RpcDef::argument() const
{
	return m_argument;
}

std::string RpcDef::result() const
{
	return m_result;
}

