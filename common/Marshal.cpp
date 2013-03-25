#include "Marshal.h"

namespace catman
{
namespace common
{

MarshalException::MarshalException()
{
}

MarshalException::~MarshalException() throw()
{
}

const char* MarshalException::what() const throw()
{
	return "Exception occurred during marshaling!";
}

}
}
