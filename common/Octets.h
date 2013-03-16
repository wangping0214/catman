#ifndef OCTETS_H
#define OCTETS_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <stddef.h>
#include <stdint.h>

namespace catman
{
namespace common
{

class Octets
{
	class Rep
	{
	public:
		Rep();
		uint32_t capacity() const;
		uint32_t size() const;
		void setSize(uint32_t newSize);
		void incRef();
		void decRef();
		void* data();
		void* unique();
		void* reserve(uint32_t size);
		static Rep* create(uint32_t capacity);
	private:
		Rep* clone();
		static uint32_t alignSize(uint32_t size);
		static void* operator new(size_t size, uint32_t extra);
		static void operator delete(void *p);
	public:
		static Rep null;
	private:
		uint32_t m_capacity;
		uint32_t m_size;
		uint32_t m_ref;
		
	};
public:
	Octets();
	Octets(uint32_t size);
	Octets(const void *buff, uint32_t len);
	Octets(const void *beginPos, const void *endPos);
	Octets(const Octets &other);
	virtual ~Octets();
	Octets& operator = (const Octets &other);
	bool operator == (const Octets &other) const;
	bool operator != (const Octets &other) const;
	Octets& swap(Octets &other);
	void* begin();
	void* end();
	const void* begin() const;
	const void* end() const;
	uint32_t size() const;
	uint32_t capacity() const;
	Octets& clear();
	Octets& erase(uint32_t pos, uint32_t len);
	Octets& erase(void *beginPos, void *endPos);
	Octets& insert(void *pos, const void *buff, uint32_t len);
	Octets& insert(void *pos, const void *beginPos, const void *endPos);
	Octets& reserve(uint32_t size);
	Octets& resize(uint32_t size);
	Octets& replace(const void *buff, uint32_t len);
	void dump();
private:
	Rep* rep() const;
private:
	void *m_base;
};

}
}

#endif

