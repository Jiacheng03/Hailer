#include "Thread.h"

Thread::~Thread()
{
	Stop();
}

// 设置任务函数，并设置循环次数
bool Thread::SetRun(function<void(void)> func, int times)
{
	if (!m_started)
	{
		m_task = func;
		m_times = times;
		return true;
	}
	return false;
}

// 启动线程
bool Thread::Start(SType type)
{
	// 若未设置任务函数，或正在运行，则返回false;
	if (!m_task || m_started)
		return false;

	m_started = true;

	m_thread = make_unique<thread>(
		thread(bind(&Thread::m_threadRun, this))
		);

	if (type == SType::THREAD_DETACH)
		m_thread->detach();
	else
		m_thread->join();

	return true;
}

// 停止线程: 停止对任务的循环执行，而当前任务会正常执行
void Thread::Stop()
{	
	m_started = false;
}

// 作为线程函数，循环调用任务函数
void Thread::m_threadRun()
{
	if (!m_started || !m_task)
		return;

	// 若预设执行次数小于等于0，则持续循环，直到线程被终止
	for (int i = 0; (i < m_times || m_times<=0) && m_started; i++)
	{
		m_task();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	m_started = false;
}
