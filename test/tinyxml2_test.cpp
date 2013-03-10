#include <tinyxml2/tinyxml2.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("catman.xml");
	tinyxml2::XMLElement *rootElement =	doc.RootElement();
	cout << rootElement->Name() << endl;
	for (tinyxml2::XMLElement *elem = rootElement->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		cout << elem->Name() << " " << elem->Attribute("name")  << " " << elem->Attribute("address") << " " 
			<< elem->Attribute("port") << " " << elem->Attribute("sendbufsize") << " " 
			<< elem->Attribute("recvbufsize") << endl;
		for (const tinyxml2::XMLAttribute *attr = elem->FirstAttribute(); attr != NULL; attr = attr->Next())
			cout << attr->Name() << ":" << attr->Value() << endl;
	}
	return 0;
}
