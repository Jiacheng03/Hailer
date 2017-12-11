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

	void Start();
	void Stop();

	void Send(const char* buf, int len);
	int Recv(char* buf, int len);

private:

	Talker m_talker;

	Thread t_bkRecv, t_bkSend;
	Thread t_recv, t_send;	

	MsgList m_sendList;	
	MsgList m_recvList;
	
	void thread_resend();
	void thread_recv();

	// 计算rto，单位暂定为秒
	unsigned int getRTO();
};

