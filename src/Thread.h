/***************************************
*		class Thread
*		负责调用任务函数和启停线程
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

	// 设置任务函数，并设置循环次数
	bool SetRun(function<void(void)> func, int times = 1);

	// 启动线程: 若未设置任务函数，或正在运行，则返回失败
	bool Start(SType type = SType::THREAD_DETACH);

	// 停止线程: 停止对任务的循环执行，而当前任务会正常执行
	void Stop();

private:
	unique_ptr<thread> m_thread;
	function<void(void)> m_task;	// 任务函数

	int m_times;		// 线程中，当前任务的执行次数
	bool m_started;		// 当前线程运行状态

	// 作为线程函数，循环调用任务函数
	void m_threadRun();
};
