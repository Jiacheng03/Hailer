#pragma once

#include "Talker.h"
#include "MsgList.h"
//#include "Console.h"
#include "Thread.h"

#include <thread>
#include <condition_variable>

class Hailer
{
public:
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

	Talker m_talker;

	Thread t_bkRecv, t_bkSend;
	Thread t_recv, t_send;	

	MsgList m_sendList;	
	MsgList m_recvList;
	
	// 负责从端口发送数据
	void thread_resend();

	// 负责监听端口
	void thread_recv();

	// 计算rto，单位暂定为秒
	unsigned int getRTO();
};

