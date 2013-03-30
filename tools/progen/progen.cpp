#include "ProDef.h"
#include "RpcDataDef.h"
#include "RpcDef.h"
#include "TabString.h"
#include <catman/tinyxml2/tinyxml2.h>
#include <stdio.h>
#include <assert.h>
#include <string>

void usage()
{
	printf("Usage: progen -d protocols.xml [-o output_path]");
}

static void genProtocolStubs(const std::string &outPath, const std::string &ns, const std::map<std::string, std::string> &proNames, uint32_t tabCount)
{
	std::string filePath(outPath);
	if (!filePath.empty())
	{
		if (filePath.find_last_of('/') != (filePath.size() - 1))
			filePath += '/';
	}
	filePath += "ProtocolStubs.cpp";
	FILE *destFile = fopen(filePath.c_str(), "w+");
	assert(NULL != destFile);
	for (std::map<std::string, std::string>::const_iterator it = proNames.begin(), ie = proNames.end(); it != ie; ++ it)
		fprintf(destFile, "%s#include \"%s.h\"\n", TabString::get(tabCount), it->first.c_str());
	fprintf(destFile, "\n");
	if (!ns.empty())
	{
		fprintf(destFile, "%snamespace %s\n", TabString::get(tabCount), ns.c_str());
		fprintf(destFile, "%s{\n", TabString::get(tabCount));
		fprintf(destFile, "\n");
	}
	for (std::map<std::string, std::string>::const_iterator it = proNames.begin(), ie = proNames.end(); it != ie; ++ it)
		fprintf(destFile, "%s%s s_%s;\n", TabString::get(tabCount), it->first.c_str(), it->second.c_str());	
	if (!ns.empty())
	{
		fprintf(destFile, "\n");
		fprintf(destFile, "%s}\n", TabString::get(tabCount));
	}
	fprintf(destFile, "\n");
}

int main(int argc, char *argv[])
{
	std::string proFile;
	std::string outPath;
	int optval;
	while((optval = getopt(argc, argv, "d:o:")) != -1)
	{
		switch(optval)
		{
		case 'd':
			proFile = optarg;
			break;
		case 'o':
			outPath = optarg;
			break;
		default:
			usage();
			break;
		}
	}
	tinyxml2::XMLDocument proDoc;
	if (proDoc.LoadFile(proFile.c_str()) != 0)
		printf("Failed to load %s\n", proFile.c_str());
	tinyxml2::XMLElement *appElem = proDoc.RootElement();
	std::string ns(appElem->Attribute("namespace"));
	std::map<std::string, std::string> proNames;
	for (tinyxml2::XMLElement *elem = appElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string genType(elem->Name());
		if ("protocol" == genType)
		{
			ProDef proDef(elem);
			proDef.write(outPath, ns, uint32_t(1));
			proNames[proDef.name()] = proDef.name();
		}
		else if ("rpcdata" == genType)
		{
			RpcDataDef dataDef(elem);
			dataDef.write(outPath, ns, uint32_t(0));
		}
		else if ("rpc" == genType)
		{
			RpcDef rpcDef(elem);
			rpcDef.write(outPath, ns, uint32_t(0));
			std::string def(rpcDef.name());
			def += "(new " + rpcDef.argument() + ", new " + rpcDef.result() + ")";
			proNames[rpcDef.name()] = def;
		}
	}
	
	genProtocolStubs(outPath, ns, proNames, uint32_t(0));

	return 0;
}
