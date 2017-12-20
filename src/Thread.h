/***************************************
*		class Thread
*		�����������������ͣ�߳�
****************************************/

#pragma once

#include <thread>
#include <memory>
#include <functional>
#include <chrono>

using namespace std;

enum class SType { THREAD_JOIN, THREAD_DETACH };

class Thread
{
public:
	~Thread();

	// ������������������ѭ������
	bool SetRun(function<void(void)> func, int times = 1);

	// �����߳�: ��δ���������������������У��򷵻�ʧ��
	bool Start(SType type = SType::THREAD_DETACH);

	// ֹͣ�߳�: ֹͣ�������ѭ��ִ�У�����ǰ���������ִ��
	void Stop();

private:
	unique_ptr<thread> m_thread;
	function<void(void)> m_task;	// ������

	int m_times;		// �߳��У���ǰ�����ִ�д���
	bool m_started;		// ��ǰ�߳�����״̬

	// ��Ϊ�̺߳�����ѭ������������
	void m_threadRun();
};
