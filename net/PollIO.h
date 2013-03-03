#ifndef POLLIO_H
#define POLLIO_H

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
	virtual void pollIn() = 0;
	virtual void pollOut() {}
	virtual void pollClose() {}
	void permitRecv();
	void permitSend();
	void forbidRecv();
	void forbidSend();
	void close();
	int fd() const
	{
		return m_fd;
	}
	int event() const
	{
		return m_event;
	}
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

