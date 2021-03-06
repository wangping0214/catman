#ifndef RUNNABLE_H
#define RUNNABLE_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

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
