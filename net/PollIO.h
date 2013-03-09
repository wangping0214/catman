#ifndef POLLIO_H
#define POLLIO_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

namespace catman
{
namespace net
{

enum
{
	POLLCLOSE = 0x80000000
};

class PollIO
{
public:
	~PollIO();
	/* trigger by POLLIN event from socket */
	virtual void pollIn() = 0;
	/* trigger by POLLOUT event from sockt */
	virtual void pollOut() = 0;
	/* Detect close event from Session */
	virtual void detectCloseEvent() = 0;
	void permitRecv();
	void permitSend();
	void forbidRecv();
	void forbidSend();
	void close();
	int fileDescriptor() const;
	int event() const;
protected:
	PollIO(int fd);
protected:
	int m_fd;
private:
	int m_event;
};

}
}

#endif

