#include <catman/common/OctetsStream.h>
#include <string.h>

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

bool OctetsStream::atEnd()
{
	return m_pos == m_data.size();
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

OctetsStream& OctetsStream::pushBytes(const char *buffer, size_t len)
{
	m_data.insert(m_data.end(), buffer, len);
	return *this;
}

// << operator overload end

// >> operator overload begin 

const OctetsStream& OctetsStream::operator >> (bool &b) const
{
	b = popUChar();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (char &c) const
{
	c = popUChar();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (unsigned char &uc) const 
{
	uc = popUChar();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (short &s) const
{
	s = popUShort();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (unsigned short &us) const
{
	us = popUShort();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (int &i) const 
{
	i = popUInt();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (unsigned int &ui) const
{
	ui = popUInt();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (long &l) const
{
	l = popULong();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (unsigned long &ul) const
{
	ul = popULong();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (float &f) const 
{
	unsigned int ui = popUInt();
	f = *(float*)(&ui);
	return *this;
}

const OctetsStream& OctetsStream::operator >> (double &d) const 
{
	unsigned long ul = popULong();
	d = *(double*)(&ul);
	return *this;
}

const OctetsStream& OctetsStream::operator >> (Marshal &marshal) const
{
	return marshal.unmarshal(*this);
}

const OctetsStream& OctetsStream::operator >> (Octets &octets) const
{
	size_t len;
	operator >> (len);
	if (len > m_data.size() - m_pos)
		; // TODO exceptional
	octets.replace((char*)m_data.begin() + m_pos, len);
	m_pos += len;
	return *this;
}

const OctetsStream& OctetsStream::popBytes(char *buff, size_t len) const
{
	if (len > m_data.size() - m_pos)
		; // TODO exceptional
	memcpy(buff, (char*)m_data.begin() + m_pos, len);
	m_pos += len;
	return *this;
}

// >> operator overload end

unsigned char OctetsStream::popUChar() const
{
	unsigned char c;
	popT(c);
	return c;
}

unsigned short OctetsStream::popUShort() const
{
	unsigned short s;
	popT(s);
	return s;
}

unsigned int OctetsStream::popUInt() const
{
	unsigned int i;
	popT(i);
	return i;
}

unsigned long OctetsStream::popULong() const
{
	unsigned long l;
	popT(l);
	return l;
}

}
}
