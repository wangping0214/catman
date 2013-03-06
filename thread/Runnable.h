#ifndef RUNNABLE_H
#define RUNNABLE_H

namespace catman
{
namespace thread
{

class Runnable
{
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};

}
}

#endif
