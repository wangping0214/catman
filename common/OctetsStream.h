#ifndef OCTETSSTREAM_H
#define OCTETSSTREAM_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/Marshal.h>
#include <catman/common/Octets.h>
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
	enum UnmarshalTXN
	{
		TXNBegin,
		TXNCommit,
		TXNRollback
	};
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
	void insert(void *pos, const void *buff, uint32_t len);
	void insert(void *pos, const void *beginPos, const void *endPos);
	void erase(uint32_t pos, uint32_t len);
	void erase(void *beginPos, void *endPos);
	void clear();
	bool atEnd();
	uint32_t size() const;

	// << operator overload begin
	OctetsStream& operator << (bool b);
	OctetsStream& operator << (int8_t c);
	OctetsStream& operator << (uint8_t uc);
	OctetsStream& operator << (int16_t s);
	OctetsStream& operator << (uint16_t us);
	OctetsStream& operator << (int32_t i);
	OctetsStream& operator << (uint32_t ui);
	OctetsStream& operator << (int64_t l);
	OctetsStream& operator << (uint64_t ul);
	OctetsStream& operator << (float f);
	OctetsStream& operator << (double d);
	OctetsStream& operator << (const Marshal &marshal);
	OctetsStream& operator << (const Octets &octets);
	OctetsStream& pushBytes(const char *buff, uint32_t len);
	template<typename T> OctetsStream& operator << (const std::basic_string<T> &str)
	{
		uint32_t byteSize = str.size() * sizeof(T);
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

	OctetsStream& operator >> (UnmarshalTXN txn);
	const OctetsStream& operator >> (bool &b) const;
	const OctetsStream& operator >> (int8_t &c) const;
	const OctetsStream& operator >> (uint8_t &uc) const;
	const OctetsStream& operator >> (int16_t &s) const;
	const OctetsStream& operator >> (uint16_t &us) const;
	const OctetsStream& operator >> (int32_t &i) const;
	const OctetsStream& operator >> (uint32_t &ui) const;
	const OctetsStream& operator >> (int64_t &l) const;
	const OctetsStream& operator >> (uint64_t &ul) const;
	const OctetsStream& operator >> (float &f) const;
	const OctetsStream& operator >> (double &d) const;
	const OctetsStream& operator >> (Marshal &marshal) const;
	const OctetsStream& operator >> (Octets &octets) const;
	const OctetsStream& popBytes(char *buff, uint32_t len) const;	
	template<typename T> const OctetsStream& operator >> (std::basic_string<T> &str) const
	{
		uint32_t size;
		operator >> (size);
		uint32_t ts = size / sizeof(T);
		str.assign((T*)((char*)m_data.begin() + m_pos), ts);
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
	unsigned char pop_uint8_t() const;
	unsigned short pop_uint16_t() const;
	unsigned int pop_uint32_t() const;
	unsigned long pop_uint64_t() const;
private:
	Octets m_data;
	mutable uint32_t m_pos;	// For output
	mutable uint32_t m_txnPos; 	// For transaction
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
	uint32_t size;
	stream >> size;
	for (uint32_t i = 0; i < size; ++ size)
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
