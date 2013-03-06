#include "OctetsStream.h"

namespace catman
{
namespace common
{

OctetsStream::OctetsStream() : m_pos(0)
{
}

OctetsStream::OctetsStream(const Octets &data) : m_data(data), m_pos(0)
{
}

OctetsStream::OctetsStream(const OctetsStream &other) : m_data(other.m_data), pos(0)
{
}

OctetsStream::~OctetsStream()
{
}

OctetsStream& OctetsStream::operator = (const OctetsStream &other)
{
	if (this != &other)
	{
		m_data = other.m_data;
		m_pos = other.m_pos;
	}
	return *this;
}

bool OctetsStream::operator == (const OctetsStream &other)
{
	return (this == &other) || (m_data == other.m_data)
}

bool OctetsStream::operator != (const OctetsStream &other)
{
	return ! operator ==(other);
}

OctetsStream::operator Octets&()
{
	return m_data;
}

OctetsStream::operator const Octets&() const
{
	return m_data;
}

unsigned char OctetsStream::popUChar()
{
	unsigned char c;
	popBytes(c);
	return c;
}

unsigned short OctetsStream::popUShort()
{
	unsigned short s;
	popBytes(s);
	return s;
}

unsigned int OctetsStream::popUInt()
{
	unsigned int i;
	popBytes(i);
	return i;
}

unsigned long OctetsStream::popULong()
{
	unsigned long l;
	popBytes(l);
	return l;
}

}
}
