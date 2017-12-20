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
	// ��ʼ�����ݷ���ģ��
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

	Talker m_talker;	// ����������

	Thread t_bkRecv, t_bkSend;		// �̹߳�����������m_talker���շ��ӿ�
	Thread t_recv, t_send;			// �̹߳��������׼�����������Ϣ���еĴ���

	MsgList m_sendList;		// ��ʱ�ش�����
	MsgList m_recvList;		// ������Ϣ�Ķ���
	
	// ����Ӷ˿ڷ�������
	void thread_resend();

	// ��������˿�
	void thread_recv();

	// ����rto����λ�ݶ�Ϊ��
	unsigned int getRTO();
};

