#include <catman/thread/ThreadPool.h>
#include <stdio.h>
#include <unistd.h>

class LogTask : public catman::thread::Runnable
{
public:
	LogTask(/*catman::thread::ThreadPool &pool*/) /*: m_poolRef(pool)*/
	{
		m_id = ++ id;	
	}
	virtual void run()
	{
		printf("thread_tid = %lu, task_id = %d\n", pthread_self(), m_id);
		sleep(1);
		//m_poolRef.execute(this);
		catman::thread::ThreadPool::instance().execute(this);
	}
private:
//	catman::thread::ThreadPool &m_poolRef;
	int m_id;
	static int id;
};

int LogTask::id = 0;

int main(int argc, char *argv[])
{
	catman::thread::ThreadPool &pool = catman::thread::ThreadPool::instance();
	LogTask task1;
	LogTask task2;
	LogTask task3;
	LogTask task4;
	pool.execute(&task1);
	pool.execute(&task2);
	pool.execute(&task3);
	pool.execute(&task4);
	pool.start();
}

