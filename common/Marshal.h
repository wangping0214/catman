#ifndef MARSHAL_H
#define MARSHAL_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

namespace catman
{
namespace common
{

class Marshal
{
public:
	virtual OctetsStream& marshal(OctetsStream &stream) const = 0;
	virtual const OctetsStream& unmarshal(const OctetsStream &stream) const = 0;
	virtual ~Marshal() {}
};

}
}

#endif
