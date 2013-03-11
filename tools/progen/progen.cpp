#include "ProDef.h"
#include <catman/tinyxml2/tinyxml2.h>
#include <stdio.h>
#include <assert.h>
#include <string>

void usage()
{
	printf("Usage: progen protocols.xml [output_path]");
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		usage();
		return 0;
	}
	tinyxml2::XMLDocument proDoc;
	if (proDoc.LoadFile(argv[1]) != 0)
		printf("Failed to load %s\n", argv[1]);
	tinyxml2::XMLElement *appElem = proDoc.RootElement();
	std::string ns(appElem->Attribute("namespace"));
	for (tinyxml2::XMLElement *elem = appElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string genType(elem->Name());
		if ("protocol" == genType)
		{
			ProDef proDef(elem);
			proDef.write(std::string(), 1);
		}
		// else if
	}
	
	return 0;
}