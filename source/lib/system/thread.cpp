
#include "thread.h"


bool Thread::create(unsigned (__stdcall* func)(void*), void* arg)
{
	if(handle)
		return false;
	handle = (HANDLE)::_beginthreadex(NULL, 0, func, arg, CREATE_SUSPENDED, &id);
	return (handle)? true : false;
}

bool Thread::close()
{
	if(!handle)
		return false;
	return (::CloseHandle(handle))? true : false;
}

bool Thread::join(unsigned long msec)
{
	if(!handle)
		return false;
	if(::WaitForSingleObject(handle, msec) != WAIT_OBJECT_0)
		return false;
	count = 1;
	return true;
}

bool Thread::resume()
{
	if(!handle || ((count - 1) != 0))
		return false;
	::ResumeThread(handle);
	count--;
	return true;
}

bool Thread::suspend()
{
	if(!handle || ((count + 1) != 1))
		return false;
	::SuspendThread(handle);
	count++;
	return true;
}

/////////////////

WorkPile::WorkPile() : enable(false), max_thread(0)
{
 #ifdef USE_THREAD_AFFINITY_MASK
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	max_cpu = sys_info.dwNumberOfProcessors;
 #endif // USE_THREAD_AFFINITY_MASK
}

WorkPile::~WorkPile()
{
	enable = false;
	for(std::list<Thread*>::iterator it = thread_list.begin(); it != thread_list.end(); it++)
	{
		(*it)->join();
		(*it)->close();
		delete (*it);
	}
}

/*!
	@brief	ワーカースレッドの起動
 */
void WorkPile::start(unsigned (__stdcall* worker_thread)(void*), std::size_t max_thread)
{
	// 生成
	this->max_thread = max_thread;

	for(std::size_t i = 0; i < max_thread; i++)
	{
		Thread* thread = new Thread();
		thread->create(worker_thread, this);
		thread_list.push_back(thread);
 #ifdef USE_THREAD_AFFINITY_MASK
		SetThreadAffinityMask(thread->get_handle(), 1<<(i%max_cpu));
 #endif // USE_THREAD_AFFINITY_MASK
	}
	// 起動させる
	for(std::list<Thread*>::iterator it = thread_list.begin(); it != thread_list.end(); it++)
		(*it)->resume();

	enable = true;
}

void WorkPile::request(Work* work)
{
	work_queue.push(work);
	left_work_list.push_back(work);
}

Work* WorkPile::get_work()
{
	Work* work = NULL;
	if(!work_queue.empty())
	{
		work = work_queue.front();
		work_queue.pop();
	}
	return work;
}

std::size_t WorkPile::get_left_work()
{
	std::list<Work*>::iterator it = left_work_list.begin();
	while(it != left_work_list.end())
	{
		if((*it)->get_status() == Work::Status_Completed)
		{
			it = left_work_list.erase(it);
			continue;
		}
		it++;
	}
	return left_work_list.size();
}
