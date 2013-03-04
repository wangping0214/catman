#ifndef OCTETS_H
#define OCTETS_H

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
		void incRef();
		void decRef();
		void* data();
		void* unique();
		void* reserve(size_t size);
		static Rep* create(size_t capacity);
	private:
		Rep* clone();
		static size_t alignSize(size_t size);
		static void* operator new(size_t size, size_t extra);
		static void* operator delete(void *p);
	public:
		size_t m_capacity;
		size_t m_size;
		size_t m_ref;
		
		static Rep null;
	};
public:
	Octets();
	Octets(size_t size);
	Octets(const void *buff, size_t len);
	Octets(const void *beginPos, const void *endPos);
	Octets(const Octets &other);
	virtual ~Octets();
	Octets& operator = (const Octets &other);
	bool operator == (const Octets &other);
	bool operator != (const Octets &other);
	Octets& swap(Octets &other);
	void* begin();
	void* end();
	const void* begin() const;
	const void* end() const;
	size_t size() const;
	size_t capacity() const;
	Octets& clear();
	Octets& erase(size_t pos, size_t len);
	Octets& erase(void *beginPos, void *endPos);
	Octets& insert(void *pos, const void *buff, size_t len);
	Octets& insert(void *pos, const void *beginPos, const void *endPos);
	Octets& resize(size_t size);
private:
	Rep *rep() const;
	void unique();
private:
	void *m_base;
};

}
}

#endif

