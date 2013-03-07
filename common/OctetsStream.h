#ifndef OCTETSSTREAM_H
#define OCTETSSTREAM_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include "Marshal.h"
#include "Octets.h"
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
 
namespace catman
{
namespace common
{

template<typename T> T& RemoveConst(const T &t)
{
	return const_cast<T&>(t);
}

template<typename container>
class STLContainer : public Marshal
{
public:
	explicit STLContainer(container &c) : m_containerPtr(&c) {}
	OctetsStream& marshal(OctetsStream &stream) const;
	const OctetsStream& unmarshal(const OctetsStream &stream);
private:
	container *m_containerPtr;
};

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
	bool atEnd();

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
	OctetsStream& pushBytes(const char *buff, size_t len);
	template<typename T> OctetsStream& operator << (const std::basic_string<T> &str)
	{
		size_t byteSize = str.size() * sizeof(T);
		operator << (byteSize);
		insert(m_data.end(), str.c_str(), byteSize);
		return *this;
	}
	template<typename KT, typename VT> OctetsStream& operator << (const std::pair<KT, VT> &p)
	{
		operator << (p.first);
		operator << (p.second);
		return *this;
	}
	template<typename T> OctetsStream& operator << (const std::vector<T> &vec)
	{
		operator << (STLContainer<std::vector<T> >(RemoveConst(vec)));
		return *this;
	}
	template<typename T> OctetsStream& operator << (const std::list<T> &tlist)
	{
		operator << (STLContainer<std::list<T> >(RemoveConst(tlist)));
		return *this;
	}
	template<typename T> OctetsStream& operator << (const std::deque<T> tdeque)
	{
		operator << (STLContainer<std::deque<T> >(RemoveConst(tdeque)));
		return *this;
	}
	template<typename KT, typename VT> OctetsStream& operator << (const std::map<KT, VT> kv)
	{
		operator << (STLContainer<std::map<KT, VT> >(RemoveConst(kv)));
		return *this;
	}
	// << operator overload end

	const OctetsStream& operator >> (bool &b) const;
	const OctetsStream& operator >> (char &c) const;
	const OctetsStream& operator >> (unsigned char &uc) const;
	const OctetsStream& operator >> (short &s) const;
	const OctetsStream& operator >> (unsigned short &us) const;
	const OctetsStream& operator >> (int &i) const;
	const OctetsStream& operator >> (unsigned int &ui) const;
	const OctetsStream& operator >> (long &l) const;
	const OctetsStream& operator >> (unsigned long &ul) const;
	const OctetsStream& operator >> (float &f) const;
	const OctetsStream& operator >> (double &d) const;
	const OctetsStream& operator >> (Marshal &marshal) const;
	const OctetsStream& operator >> (Octets &octets) const;
	const OctetsStream& popBytes(char *buff, size_t len) const;	
	template<typename T> const OctetsStream& operator >> (std::basic_string<T> &str) const
	{
		size_t size;
		operator >> (size);
		str.assign((T*)((char*)m_data.begin() + m_pos), size / sizeof(T));
		m_pos += size;
		return *this;
	}
	template<typename KT, typename VT> const OctetsStream& operator >> (std::pair<KT, VT> &p) const
	{
		operator >> (p.first);
		operator >> (p.second);
		return *this;
	}
	template<typename T> const OctetsStream& operator >> (std::vector<T> &vec) const
	{
		operator >> (STLContainer<std::vector<T> >(vec));
		return *this;
	}
	template<typename T> const OctetsStream& operator >> (std::list<T> &li) const
	{
		operator >> (STLContainer<std::list<T> >(li));
		return *this;
	}
	template<typename T> const OctetsStream& operator >> (std::deque<T> &tdeque) const
	{
		operator >> (STLContainer<std::deque<T> >(tdeque));
		return *this;
	}
	template<typename KT, typename VT> const OctetsStream& operator >> (std::map<KT, VT> &tmap) const
	{
		operator >> (STLContainer<std::map<KT, VT> >(tmap));
		return *this;
	}
private:
	template<typename T> OctetsStream& pushT(T val)
	{
		m_data.insert(m_data.end(), &val, sizeof(T));
		return *this;
	}
	template<typename T> void popT(T &val) const
	{
		if (m_pos + sizeof(T) > m_data.size())
			;
		val = *(T*)((char*)m_data.begin() + m_pos);
		m_pos += sizeof(T);
	}
	unsigned char popUChar() const;
	unsigned short popUShort() const;
	unsigned int popUInt() const;
	unsigned long popULong() const;
private:
	Octets m_data;
	mutable size_t m_pos;	// For output
};

template<typename container> OctetsStream& STLContainer<container>::marshal(OctetsStream &stream) const
{
	stream << m_containerPtr->size();
	for (typename container::const_iterator it = m_containerPtr->begin(), ie = m_containerPtr->end(); it != ie; ++ it)
		stream << *it;
	return stream;
}

template<typename container> const OctetsStream& STLContainer<container>::unmarshal(const OctetsStream &stream)
{
	m_containerPtr->clear();
	size_t size;
	stream >> size;
	for (size_t i = 0; i < size; ++ size)
	{
		typename container::value_type tmp;
		stream >> tmp;
		m_containerPtr->insert(tmp);
	}
	return stream;
}

}
}

#endif
