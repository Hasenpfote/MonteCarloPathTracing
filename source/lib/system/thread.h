//==================================================================================
/*!
	@file	thread.h
    @brief  スレッド
    @author M.Morimoto
    @date   2010/03/10
	@note	work pile アルゴリズム
			Win32 環境依存
			二重起動されると不味いので対策を考慮すること・・・ mutex とかね
 */
//==================================================================================
#ifndef __THREAD_H_
#define __THREAD_H_

#include <windows.h>
#include <process.h>
#include <queue>
#include <list>

#define USE_THREAD_AFFINITY_MASK

/*!
	@brief	単純なスレッドラッパークラス
	@class	Thread
 */
class Thread
{
public:
	Thread() : handle(NULL), id(0), count(1) {}
	virtual ~Thread(){}

	bool create(unsigned (__stdcall* func)(void*), void* arg);
	bool close();
	bool join(unsigned long msec = INFINITE);
	bool resume();
	bool suspend();
	HANDLE get_handle(){ return handle; }
	unsigned int get_id(){ return id; }

private:
	HANDLE			handle;
	unsigned int	id;
	int				count;
};

/*!
	@brief	クリティカルセクションラッパークラス
	@class	CriticalSection
 */
class CriticalSection
{
public:
	CriticalSection(){ InitializeCriticalSection(&cs); }
	~CriticalSection(){ DeleteCriticalSection(&cs); }
	void lock(){ EnterCriticalSection(&cs); }
	void unlock(){ LeaveCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
};

/*!
	@brief	スレッド生成失敗時の例外クラス
	@class	thread_resource_error
 */
class thread_resource_error : public std::runtime_error
{
public:
	thread_resource_error() : std::runtime_error("error"){}
};

/*!
	@brief	WorkPile 用作業クラス
	@class	Work
	@note	WorkPile に要求を出すときの基底クラス
 */
class Work
{
public:
	enum Status
	{
		Status_NotStarted,
		Status_Start,
		Status_Completed
	};

public:
	Work() : status(Status_NotStarted) {}
	virtual ~Work(){}
	Status get_status(){ return status; }
	void set_status(Status s){ status = s; }

protected:
	Status status;
};

/*!
	@brief	WorkPile アルゴリズムによるスレッド
	@class	WorkPile
	@note	基本的に走りっぱなし
 */
class WorkPile
{
public:
	WorkPile();
	~WorkPile();

	void start(unsigned (__stdcall* func)(void*), std::size_t max_thread);
	void request(Work* work);
	bool is_enable(){ return enable; }
	Work* get_work();
	std::size_t get_left_work();

private:
	bool				enable;
	std::size_t			max_thread;
	std::list<Thread*>	thread_list;
	std::queue<Work*>	work_queue;
	std::list<Work*>	left_work_list;
 #ifdef USE_THREAD_AFFINITY_MASK
	DWORD				max_cpu;
 #endif // USE_THREAD_AFFINITY_MASK
};

#endif // !__THREAD_H_