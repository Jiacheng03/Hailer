#include "Hailer.h"
#include <thread>
#include <iostream>

Hailer::Hailer(unsigned short myport, const char* peerip, unsigned short peerPort)
:m_talker(myport, peerip, peerPort)
{
}

// ��ʼ����
void Hailer::Start()
{	
	t_bkRecv.SetRun(bind(&Hailer::thread_recv, this), 0);
	t_bkSend.SetRun(bind(&Hailer::thread_resend, this), 0);
	t_bkRecv.Start();
	t_bkSend.Start();

	t_send.SetRun([this](){
		char buf[MAXBYTE] = { 0 };
		cin.getline(buf, MAXBYTE);
		if (strcmp(buf, "exit") == 0)
		{
			Stop();
		}
		Send(buf, strlen(buf) + 1);
	}, 0);
	t_send.Start();


	t_recv.SetRun([this](){
		char buf[MAXBYTE] = { 0 };
		if (Recv(buf, sizeof(buf)) >= 0)
			printf("from peer: %s\n", buf);
	}, 0);
	t_recv.Start(SType::THREAD_JOIN);
}

// ������������
void Hailer::Stop()
{
	// ��ֹ�߳�
	t_bkRecv.Stop();
	t_bkSend.Stop();

	t_send.Stop();
	t_recv.Stop();

	// �����Ϣ����ռ�õ���
	m_sendList.Stop();
	m_recvList.Stop();	

	this_thread::sleep_for(chrono::milliseconds(300));
	exit(0);
}

//�������ݣ��������ݼ�����Ϣ����
void Hailer::Send(const char* buf, int len)
{
	Msg * pMsg = (Msg*)new char[sizeof(Msg)+len];
	memset(pMsg, 0, sizeof(Msg)+len);	
	pMsg->time = time(NULL);	
	pMsg->size = len;
	memmove(pMsg->data, buf, len);

	shared_ptr<Msg> shared_pMsg(pMsg);
	m_sendList.Push_front(shared_pMsg);	
}

// �������ݣ�������Ϣ����ȡ����
int Hailer::Recv(char* buf, int len)
{
	shared_ptr<Msg> pMsg = m_recvList.Pop();
	if (pMsg->size < len)
		len = pMsg->size;
	memmove(buf, pMsg->data, len);
	return len;
}

// ����Ӷ˿ڷ�������
void Hailer::thread_resend()
{		
	// �������ݣ�ֱ����Ϣ����Ϊ��
	while (m_sendList.CheckTime(time(NULL)))
	{
		shared_ptr<Msg> pMsg = m_sendList.Pop();
		m_talker.Send((char*)pMsg.get(), sizeof(Msg)+pMsg->size);
		pMsg->time = time(NULL);
		pMsg->rto = getRTO();				// ��ʱ��Ϊ2��
		m_sendList.InsertByOrder(pMsg);
	}
}

// ��������˿�
void Hailer::thread_recv()
{
	char buf[1024] = { 0 };
	int size = m_talker.Recv(buf, sizeof(buf));
	if (size == -WSAECONNRESET)		// ���󣺶Է�δ�����˿�
	{
		printf("%s\n", "peer is not online.");
		return;
	}
	else if (size <= 0)		// ��������
	{		
		return;
	}

	Msg* pMsg = (Msg*)new char[size];
	memmove(pMsg, buf, size);

	shared_ptr<Msg> shared_pMsg(pMsg);
	if (shared_pMsg->time + shared_pMsg->rto < time(NULL))	// ���ѳ�ʱ�������			
	{
		return;
	}
	else if (shared_pMsg->ACK)		// ���յ�����ack
	{
		m_sendList.Erase(shared_pMsg->seq);
	}
	else  // �յ��������ݰ�
	{
		m_recvList.Push_back(shared_pMsg);

		// ����ACK			
		shared_ptr<Msg> ackMsg(new Msg);
		ackMsg->seq = shared_pMsg->seq;
		ackMsg->time = shared_pMsg->time;
		ackMsg->rto = shared_pMsg->rto;
		ackMsg->size = 0;
		ackMsg->ACK = true;
		m_talker.Send((const char*)(ackMsg.get()), sizeof(Msg));
	}
}

// ����rto����λ�ݶ�Ϊ��
unsigned int Hailer::getRTO()
{
	return 2;	// ��ʱ��Ϊ2��
}
