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

void OctetsStream::insert(void *pos, const void *buff, uint32_t len)
{
	m_data.insert(pos, buff, len);
}

void OctetsStream::insert(void *pos, const void *beginPos, const void *endPos)
{
	m_data.insert(pos, beginPos, endPos);
}

void OctetsStream::erase(uint32_t pos, uint32_t len)
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

uint32_t OctetsStream::size() const
{
	return m_data.size();
}

// >> operator overload begin

OctetsStream& OctetsStream::operator << (bool b)
{
	return pushT(b);
}

OctetsStream& OctetsStream::operator << (int8_t c)
{
	return pushT(c);
}

OctetsStream& OctetsStream::operator << (uint8_t uc)
{
	return pushT(uc);
}

OctetsStream& OctetsStream::operator << (int16_t s)
{
	return pushT(s);
}

OctetsStream& OctetsStream::operator << (uint16_t us)
{
	return pushT(us);
}

OctetsStream& OctetsStream::operator << (int32_t i)
{
	return pushT(i);
}

OctetsStream& OctetsStream::operator << (uint32_t ui)
{
	return pushT(ui);
}

OctetsStream& OctetsStream::operator << (int64_t l)
{
	return pushT(l);
}

OctetsStream& OctetsStream::operator << (uint64_t ul)
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

OctetsStream& OctetsStream::pushBytes(const char *buffer, uint32_t len)
{
	m_data.insert(m_data.end(), buffer, len);
	return *this;
}

// << operator overload end

// >> operator overload begin 

OctetsStream& OctetsStream::operator >> (UnmarshalTXN txn)
{
	switch (txn)
	{
	case TXNBegin:
		m_txnPos = m_pos;
		break;
	case TXNRollback:
		m_pos = m_txnPos;
		break;
	case TXNCommit:
		if (m_pos >= 16384)
		{
			erase(0, m_pos);
			m_pos = 0;
		}
	}
	return *this;
}

const OctetsStream& OctetsStream::operator >> (bool &b) const
{
	b = pop_uint8_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (int8_t &c) const
{
	c = pop_uint8_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (uint8_t &uc) const 
{
	uc = pop_uint8_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (int16_t &s) const
{
	s = pop_uint16_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (uint16_t &us) const
{
	us = pop_uint16_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (int32_t &i) const 
{
	i = pop_uint32_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (uint32_t &ui) const
{
	ui = pop_uint32_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (int64_t &l) const
{
	l = pop_uint64_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (uint64_t &ul) const
{
	ul = pop_uint64_t();
	return *this;
}

const OctetsStream& OctetsStream::operator >> (float &f) const 
{
	uint32_t ui = pop_uint32_t();
	f = *(float*)(&ui);
	return *this;
}

const OctetsStream& OctetsStream::operator >> (double &d) const 
{
	uint64_t ul = pop_uint64_t();
	d = *(double*)(&ul);
	return *this;
}

const OctetsStream& OctetsStream::operator >> (Marshal &marshal) const
{
	return marshal.unmarshal(*this);
}

const OctetsStream& OctetsStream::operator >> (Octets &octets) const
{
	uint32_t len;
	operator >> (len);
	if (len > m_data.size() - m_pos)
		throw MarshalException(); 
	octets.replace((char*)m_data.begin() + m_pos, len);
	m_pos += len;
	return *this;
}

const OctetsStream& OctetsStream::popBytes(char *buff, uint32_t len) const
{
	if (len > m_data.size() - m_pos)
		throw MarshalException(); 
	memcpy(buff, (char*)m_data.begin() + m_pos, len);
	m_pos += len;
	return *this;
}

// >> operator overload end

unsigned char OctetsStream::pop_uint8_t() const
{
	unsigned char c;
	popT(c);
	return c;
}

unsigned short OctetsStream::pop_uint16_t() const
{
	unsigned short s;
	popT(s);
	return s;
}

unsigned int OctetsStream::pop_uint32_t() const
{
	unsigned int i;
	popT(i);
	return i;
}

unsigned long OctetsStream::pop_uint64_t() const
{
	unsigned long l;
	popT(l);
	return l;
}

}
}
