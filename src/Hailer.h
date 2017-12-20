#pragma once

#include "Talker.h"
#include "MsgList.h"
#include "Thread.h"

#include <thread>
#include <condition_variable>

#include <string.h>

class Hailer
{
public:
	// 初始化数据发送模块
	Hailer(unsigned short myport, const char* peerip, unsigned short peerPort);

	// 开始服务
	void Start();
	
	// 结束整个进程
	void Stop();

	//发送数据，即将数据加入消息队列
	void Send(const char* buf, int len);

	// 接收数据，即从消息队列取数据
	string Recv();

private:

	Talker m_talker;	// 负责发送数据

	Thread t_bkRecv, t_bkSend;		// 线程管理：持续调用m_talker的收发接口
	Thread t_recv, t_send;			// 线程管理：负责标准输入输出和消息队列的处理

	MsgList m_sendList;		// 超时重传队列
	MsgList m_recvList;		// 接收消息的队列
	
	// 负责从端口发送数据
	void thread_resend();

	// 负责监听端口
	void thread_recv();

	// 计算rto，单位暂定为秒
	unsigned int getRTO();
};

