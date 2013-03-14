#ifndef FIELD_H
#define FIELD_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/tinyxml2/tinyxml2.h>
#include <string>
#include <stdio.h>

class Field
{
public:
	Field(const tinyxml2::XMLElement *fieldElem);
	~Field();
	void write(FILE* destFile, int tabCount) const;
	const std::string& name() const;
	const std::string& type() const;
	const std::string& attr() const;
private:
	std::string m_name;
	std::string m_type;
	std::string m_attr;
};

#endif
