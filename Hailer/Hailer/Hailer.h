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

	// ��ʼ����
	void Start();
	
	// ������������
	void Stop();

	//�������ݣ��������ݼ�����Ϣ����
	void Send(const char* buf, int len);

	// �������ݣ�������Ϣ����ȡ����
	string Recv();

private:

	Talker m_talker;

	Thread t_bkRecv, t_bkSend;
	Thread t_recv, t_send;	

	MsgList m_sendList;	
	MsgList m_recvList;
	
	// ����Ӷ˿ڷ�������
	void thread_resend();

	// ��������˿�
	void thread_recv();

	// ����rto����λ�ݶ�Ϊ��
	unsigned int getRTO();
};

