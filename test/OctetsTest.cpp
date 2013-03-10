#include <catman/common/OctetsStream.h>
#include <catman/common/Octets.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	catman::common::OctetsStream os;
	size_t in = 4;
	os << in;
	size_t out = 0;
	os >> out;
	printf("%lu\n", out);
	return 0;
}
