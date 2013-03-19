#include "ProDef.h"
#include "RpcDataDef.h"
#include "RpcDef.h"
#include <catman/tinyxml2/tinyxml2.h>
#include <stdio.h>
#include <assert.h>
#include <string>

void usage()
{
	printf("Usage: progen -d protocols.xml [-o output_path]");
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
	for (tinyxml2::XMLElement *elem = appElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string genType(elem->Name());
		if ("protocol" == genType)
		{
			ProDef proDef(elem);
			proDef.write(outPath, uint32_t(1));
		}
		else if ("rpcdata" == genType)
		{
			RpcDataDef dataDef(elem);
			dataDef.write(outPath, uint32_t(0));
		}
		else if ("rpc" == genType)
		{
			RpcDef rpcDef(elem);
			rpcDef.write(outPath, uint32_t(0));
		}
	}
	
	return 0;
}
