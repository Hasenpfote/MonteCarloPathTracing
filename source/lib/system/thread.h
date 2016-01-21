//==================================================================================
/*!
	@file	thread.h
    @brief  �X���b�h
    @author M.Morimoto
    @date   2010/03/10
	@note	work pile �A���S���Y��
			Win32 ���ˑ�
			��d�N�������ƕs�����̂ő΍���l�����邱�ƁE�E�E mutex �Ƃ���
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
	@brief	�P���ȃX���b�h���b�p�[�N���X
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
	@brief	�N���e�B�J���Z�N�V�������b�p�[�N���X
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
	@brief	�X���b�h�������s���̗�O�N���X
	@class	thread_resource_error
 */
class thread_resource_error : public std::runtime_error
{
public:
	thread_resource_error() : std::runtime_error("error"){}
};

/*!
	@brief	WorkPile �p��ƃN���X
	@class	Work
	@note	WorkPile �ɗv�����o���Ƃ��̊��N���X
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
	@brief	WorkPile �A���S���Y���ɂ��X���b�h
	@class	WorkPile
	@note	��{�I�ɑ�����ςȂ�
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