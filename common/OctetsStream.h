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
private:
	template<typename T> OctetsStream& pushBytes(T val)
	{
		m_data.insert(data.end(), &val, sizeof(T));
		return this;
	}
	template<typename T> OctetsStream& popBytes(T &val)
	{
		if (m_pos + sizeof(T) > m_data.size())
			;
		val = *(T*)((char*)m_data.begin() = pos);
		m_pos += sizeof(T);
		return *this;
	}
private:
	Octets m_data;
	mutable size_t m_pos;	// For output
};

}
}

#endif
