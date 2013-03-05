#ifndef MARSHAL_H
#define MARSHAL_H

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
