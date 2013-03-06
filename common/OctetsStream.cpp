#include "OctetsStream.h"

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

OctetsStream::OctetsStream(const OctetsStream &other) : m_data(other.m_data), pos(0)
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

bool OctetsStream::operator == (const OctetsStream &other)
{
	return (this == &other) || (m_data == other.m_data)
}

bool OctetsStream::operator != (const OctetsStream &other)
{
	return ! operator ==(other);
}

}
}