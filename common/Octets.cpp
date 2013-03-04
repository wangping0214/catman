#include "Octets.cpp"
#include <stdlib.h>

namespace catman
{
namespace common
{

Octets::Rep Octets::Rep::null;

Octets::Rep::Rep() : m_capacity(0), m_size(0), m_ref(0)
{
}

void Octets::Rep::incRef()
{
	++ m_ref;
}

void Octets::Rep::decRef()
{
	if (-- m_ref == 0)
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

void* Octets::Rep::reserve(size_t size)
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

Rep* Octets::Rep::create(size_t capacity)
{
	Rep *rep = new (capacity) Rep;
	rep->m_capacity = capacity;
	rep->m_size = 0;
	rep->m_ref = 1;
	return rep;
}

Rep* Octets::Rep::clone()
{
	Rep *rep = create(m_capacity);
	memcpy(rep->data(), data(), m_size);
	rep->m_size = m_size;
	return rep;
}

size_t Octets::Rep::alignSize(size_t size)
{
	size_t tmp = 16;
	while (size > tmp)
		tmp <<= 1;
	return tmp;
}

void* Octets::Rep::operator new(size_t size, size_t extra)
{
	return malloc(size + extra);
}

void* Octets::Rep::operator delete(void *p)
{
	free(p);
}

////////////////////////////////////////////////////////////////

Octets::Octets() : 

}
}