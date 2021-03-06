#include <catman/common/Octets.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace catman
{
namespace common
{

static void* fast_memmove(void *dest, const void *src, uint32_t n)
{
	char *pdest = (char*)dest;
	char *psrc = (char*)src;
	if (psrc < pdest && (psrc + n) > pdest)
		return memmove(dest, src, n);
	else
		return memcpy(dest, src, n);
}

////////////////////////////////////////////////////////////////

Octets::Rep Octets::Rep::null;

Octets::Rep::Rep() : m_capacity(0), m_size(0), m_ref(0)
{
}

uint32_t Octets::Rep::capacity() const
{
	return m_capacity;
}

uint32_t Octets::Rep::size() const
{
	return m_size;
}

void Octets::Rep::setSize(uint32_t newSize) 
{
	m_size = newSize;
}

void Octets::Rep::incRef()
{
	__sync_add_and_fetch(&m_ref, 1);
}

void Octets::Rep::decRef()
{
	if (__sync_sub_and_fetch(&m_ref, 1) == 0)
		delete this;
}

void* Octets::Rep::data()
{
	return reinterpret_cast<void*>(this + 1);
}

void* Octets::Rep::unique()
{
	if (m_ref > 1)
	{
		Rep *rep = clone();
		decRef();
		return rep->data();
	}
	return data();
}

void* Octets::Rep::reserve(uint32_t size)
{
	size = alignSize(size);
	if (size > m_capacity)
	{
		Rep *rep = create(size);
		memcpy(rep->data(), data(), m_size);
		rep->m_size = m_size;
		return rep->data();
	}
	return unique();
}

Octets::Rep* Octets::Rep::create(uint32_t capacity)
{
	Rep *rep = new (capacity) Rep;
	rep->m_capacity = capacity;
	rep->m_size = 0;
	rep->m_ref = 1;
	return rep;
}

Octets::Rep* Octets::Rep::clone()
{
	Rep *rep = create(m_capacity);
	memcpy(rep->data(), data(), m_size);
	rep->m_size = m_size;
	return rep;
}

uint32_t Octets::Rep::alignSize(uint32_t size)
{
	uint32_t tmp = 16;
	while (size > tmp)
		tmp <<= 1;
	return tmp;
}

void* Octets::Rep::operator new(size_t size, uint32_t extra)
{
	return malloc(size + extra);
}

void Octets::Rep::operator delete(void *p)
{
	free(p);
}

////////////////////////////////////////////////////////////////

Octets::Octets() : m_base(Rep::null.data())
{
	rep()->incRef();
}

Octets::Octets(uint32_t size) : m_base(Rep::create(size)->data())
{
}

Octets::Octets(const void *buffer, uint32_t len) : m_base(Rep::create(len)->data())
{
	memcpy(m_base, buffer, len);
	rep()->setSize(len);
}

Octets::Octets(const void *beginPos, const void *endPos) : m_base(Rep::create((char*)endPos - (char*)beginPos)->data())
{
	uint32_t len = (char*)endPos - (char*)beginPos;
	memcpy(m_base, beginPos, len);
	rep()->setSize(len);
}

Octets::Octets(const Octets &other) : m_base(other.m_base)
{
	rep()->incRef();
}

Octets::~Octets()
{
	rep()->decRef();
}

Octets& Octets::operator = (const Octets &other)
{
 	if (&other != this)
	{
		rep()->decRef();
		m_base = other.m_base;
		rep()->incRef();
	}
	return *this;
}

bool Octets::operator == (const Octets &other) const
{
	if (&other == this)
		return true;
	return ((size() == other.size()) && (memcmp(m_base, other.m_base, size()) == 0));
}

bool Octets::operator != (const Octets &other) const
{
	return ! operator ==(other);
}

Octets& Octets::swap(Octets &other)
{
	void *tmp = other.m_base;
	other.m_base = m_base;
	m_base = tmp;
	return *this;
}

void* Octets::begin()
{
	m_base = rep()->unique();
	return m_base;
}

void* Octets::end()
{
	m_base = rep()->unique();
	return (char*)m_base + size();
}

const void* Octets::begin() const
{
	return m_base;
}

const void* Octets::end() const
{
	return (char*)m_base + size();
}

uint32_t Octets::size() const
{
	return rep()->size();
}

uint32_t Octets::capacity() const
{
	return rep()->capacity();
}

Octets& Octets::clear()
{
	m_base = rep()->unique();
	rep()->setSize(0);
	return *this;
}

Octets& Octets::erase(uint32_t pos, uint32_t len)
{
	void *beginPos = (char*)begin() + pos;
	void *endPos = (char*)beginPos + len;
	return erase(beginPos, endPos);
}

Octets& Octets::erase(void *beginPos, void *endPos)
{
	if (beginPos != endPos)
	{
		void *tmp = m_base;
		m_base = rep()->unique();
		ptrdiff_t diff = (char*)m_base - (char*)tmp;
		if (diff)
		{
			beginPos = (char*)beginPos + diff;
			endPos = (char*)beginPos + diff;
		}
		fast_memmove(beginPos, endPos, ((char*)endPos + size()) - (char*)m_base);
		uint32_t newSize = rep()->size() - ((char*)endPos - (char*)beginPos);
		rep()->setSize(newSize);
	}
	return *this;
}

Octets& Octets::insert(void *pos, const void *buff, uint32_t len)
{
	ptrdiff_t offset = (char*)pos - (char*)m_base;
	reserve(size() + len);
	pos = (char*)m_base + offset;
	uint32_t adjust = size() - offset;
	if (adjust)
		fast_memmove((char*)pos + len, pos, len);
	fast_memmove(pos, buff, len);
	rep()->setSize(rep()->size() + len);
	return *this;
}

Octets& Octets::insert(void *pos, const void *beginPos, const void *endPos)
{
	return insert(pos, beginPos, (char*)endPos - (char*)beginPos);
}

Octets& Octets::reserve(uint32_t size)
{
	m_base = rep()->reserve(size);
	return *this;
}

Octets& Octets::resize(uint32_t size)
{
	reserve(size);
	rep()->setSize(size);
	return *this;
}

Octets& Octets::replace(const void *buff, uint32_t len)
{
	reserve(len);
	memcpy(m_base, buff, len);
	rep()->setSize(len);
	return *this;
}

void Octets::dump()
{
	const unsigned char *p = (const unsigned char*)begin();
	for (uint32_t i = 0; i < size(); ++ i)
		printf("%02x", *p++);
	printf("\n");
}

Octets::Rep* Octets::rep() const
{
	return reinterpret_cast<Rep*>(m_base) - 1;
}

}
}

