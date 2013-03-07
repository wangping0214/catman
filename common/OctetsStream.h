#ifndef OCTETSSTREAM_H
#define OCTETSSTREAM_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Octets.h"
 
namespace catman
{
namespace common
{

class Marshal;

class OctetsStream
{
public:
	OctetsStream();
	OctetsStream(const Octets &data);
	OctetsStream(const OctetsStream &other);
	virtual ~OctetsStream();
	OctetsStream& operator = (const OctetsStream &other);
	bool operator == (const OctetsStream &other) const;
	bool operator != (const OctetsStream &other) const;
	operator Octets& ();
	operator const Octets& () const;
	void* begin();
	void* end();
	const void* begin() const;
	const void* end() const;
	void insert(void *pos, const void *buff, size_t len);
	void insert(void *pos, const void *beginPos, const void *endPos);
	void erase(size_t pos, size_t len);
	void erase(void *beginPos, void *endPos);
	void clear();

	// << operator overload begin
	OctetsStream& operator << (bool b);
	OctetsStream& operator << (char c);
	OctetsStream& operator << (unsigned char uc);
	OctetsStream& operator << (short s);
	OctetsStream& operator << (unsigned short us);
	OctetsStream& operator << (int i);
	OctetsStream& operator << (unsigned int ui);
	OctetsStream& operator << (long l);
	OctetsStream& operator << (unsigned long ul);
	OctetsStream& operator << (float f);
	OctetsStream& operator << (double d);
	OctetsStream& operator << (const Marshal &marshal);
	OctetsStream& operator << (const Octets &octets);
private:
	template<typename T> OctetsStream& pushT(T val)
	{
		m_data.insert(m_data.end(), &val, sizeof(T));
		return *this;
	}
	template<typename T> OctetsStream& popT(T &val)
	{
		if (m_pos + sizeof(T) > m_data.size())
			;
		val = *(T*)((char*)m_data.begin() + m_pos);
		m_pos += sizeof(T);
		return *this;
	}
	unsigned char popUChar();
	unsigned short popUShort();
	unsigned int popUInt();
	unsigned long popULong();
private:
	Octets m_data;
	mutable size_t m_pos;	// For output
};

}
}

#endif
