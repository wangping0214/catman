#include "OctetsStream.h"
#include "Marshal.h"

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

OctetsStream::operator const Octets&() const
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

// >> operator overload begin

OctetsStream& OctetsStream::operator << (bool b)
{
	return pushT(b);
}

OctetsStream& OctetsStream::operator << (char c)
{
	return pushT(c);
}

OctetsStream& OctetsStream::operator << (unsigned char uc)
{
	return pushT(uc);
}

OctetsStream& OctetsStream::operator << (short s)
{
	return pushT(s);
}

OctetsStream& OctetsStream::operator << (unsigned short us)
{
	return pushT(us);
}

OctetsStream& OctetsStream::operator << (int i)
{
	return pushT(i);
}

OctetsStream& OctetsStream::operator << (unsigned int ui)
{
	return pushT(ui);
}

OctetsStream& OctetsStream::operator << (long l)
{
	return pushT(l);
}

OctetsStream& OctetsStream::operator << (unsigned long ul)
{
	return pushT(ul);
}

OctetsStream& OctetsStream::operator << (float f)
{
	return pushT(f);
}

OctetsStream& OctetsStream::operator << (double d)
{
	return pushT(d);
}

OctetsStream& OctetsStream::operator << (const Marshal &marshal)
{
	marshal.marshal(*this);
	return *this;
}

OctetsStream& OctetsStream::operator << (const Octets &octets)
{
	operator << (octets.size());
	m_data.insert(m_data.end(), octets.begin(), octets.end());
	return *this;
}

// >> operator overlaod end



unsigned char OctetsStream::popUChar()
{
	unsigned char c;
	popT(c);
	return c;
}

unsigned short OctetsStream::popUShort()
{
	unsigned short s;
	popT(s);
	return s;
}

unsigned int OctetsStream::popUInt()
{
	unsigned int i;
	popT(i);
	return i;
}

unsigned long OctetsStream::popULong()
{
	unsigned long l;
	popT(l);
	return l;
}

}
}
