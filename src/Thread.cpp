#include "Thread.h"

Thread::~Thread()
{
	Stop();
}

// ������������������ѭ������
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

// �����߳�
bool Thread::Start(SType type)
{
	// ��δ���������������������У��򷵻�false;
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

// ֹͣ�߳�: ֹͣ�������ѭ��ִ�У�����ǰ���������ִ��
void Thread::Stop()
{	
	m_started = false;
}

// ��Ϊ�̺߳�����ѭ������������
void Thread::m_threadRun()
{
	if (!m_started || !m_task)
		return;

	// ��Ԥ��ִ�д���С�ڵ���0�������ѭ����ֱ���̱߳���ֹ
	for (int i = 0; (i < m_times || m_times<=0) && m_started; i++)
	{
		m_task();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	m_started = false;
}
