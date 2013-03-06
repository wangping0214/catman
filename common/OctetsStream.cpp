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

OctetsStream::OctetsStream(const OctetsStream &other) : m_data(other.m_data), m_pos(0)
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

bool OctetsStream::operator == (const OctetsStream &other) const
{
	return (this == &other) || (m_data == other.m_data);
}

bool OctetsStream::operator != (const OctetsStream &other) const
{
	return ! operator ==(other);
}

OctetsStream::operator Octets&()
{
	return m_data;
}

void* OctetsStream::begin()
{
	return m_data.begin();
}

void* OctetsStream::end()
{
	return m_data.end();
}

const void* OctetsStream::begin() const
{
	return m_data.begin();
}

const void* OctetsStream::end() const
{
	return m_data.end();
}

void OctetsStream::insert(void *pos, const void *buff, size_t len)
{
	m_data.insert(pos, buff, len);
}

void OctetsStream::insert(void *pos, const void *beginPos, const void *endPos)
{
	m_data.insert(pos, beginPos, endPos);
}

void OctetsStream::erase(size_t pos, size_t len)
{
	m_data.erase(pos, len);
} 

void OctetsStream::erase(void *beginPos, void *endPos)
{
	m_data.erase(beginPos, endPos);
}

void OctetsStream::clear()
{
	m_data.clear();
	m_pos = 0;
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
