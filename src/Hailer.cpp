#include "Hailer.h"
#include <thread>
#include <iostream>

// ��ʼ�����ݷ���ģ��
Hailer::Hailer(unsigned short myport, const char* peerip, unsigned short peerPort)
:m_talker(myport, peerip, peerPort)
{
}

// ��ʼ����
void Hailer::Start()
{	
	// ������ȡ���ԶԶ˵�����
	t_bkRecv.SetRun(bind(&Hailer::thread_recv, this), 0);

	// ���ش����е����ݷ���
	t_bkSend.SetRun(bind(&Hailer::thread_resend, this), 0);

	t_bkRecv.Start();
	t_bkSend.Start();

	// ����Ϣ����ȡ����Ϣ����ӡ�ڿ���̨
	t_recv.SetRun([this](){
		string buf = Recv();
		if (!buf.empty())		
			printf("from peer: %s\n", buf.c_str());			
	}, 0);
	t_recv.Start();

	// �ӿ���̨��ȡ��Ϣ�������ش�����
	t_send.SetRun([this]() {
		char buf[255] = { 0 };
		cin.getline(buf, 255);
		if (strcmp(buf, "exit") == 0)
		{
			Stop();
		}
		Send(buf, strlen(buf) + 1);
	}, 0);
	t_send.Start(SType::THREAD_JOIN);
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
	Msg msg = make_shared<RealMsg>(buf, len, 0);	
	m_sendList.Push_front(msg);	
}

// �������ݣ�������Ϣ����ȡ����
string Hailer::Recv()
{
	Msg msg = m_recvList.Pop();
	return msg->m_data;
}

// ����Ӷ˿ڷ�������
void Hailer::thread_resend()
{		
	// �������ݣ�ֱ����Ϣ����Ϊ��
	while (m_sendList.CheckTime((unsigned int)time(NULL)))
	{
		Msg msg = m_sendList.Pop();
		string buf = msg->Serialize();
		m_talker.Send(buf.c_str(), buf.size());

		msg->m_head.timestamp = (unsigned int)time(NULL);	// ����ʱ���
		msg->m_head.rto = getRTO();			// ��ʱ��Ϊ����2��
		m_sendList.InsertByOrder(msg);
	}
}

// ��������˿ڣ���������m_talker��������
void Hailer::thread_recv()
{
	char buf[1024] = { 0 };
	int size = m_talker.Recv(buf, sizeof(buf));
	if (size == -ECONNRESET)		// �Է�δ�����˿�
	{
		printf("%s\n", "peer is not online.");
		return;
	}
	else if (size <= 0)		// ��������
	{		
		return;
	}

	Msg msg = make_shared<RealMsg>(string(buf, size));

	if (msg->m_head.timestamp + msg->m_head.rto < time(NULL))	// ���ѳ�ʱ�������			
	{
		return;
	}
	else if (msg->m_head.ACK)		// ���յ�����ack
	{
		m_sendList.Erase(msg->m_head.seq);
	}
	else  // �յ��������ݰ�
	{
		m_recvList.Push_back(msg);

		// ����ACK			
		Msg ackMsg( msg->GetACK());
		string ackbuf = ackMsg->Serialize();
		
		m_talker.Send(ackbuf.c_str(), ackbuf.size());
	}
}

// ����rto����λ�ݶ�Ϊ��
unsigned int Hailer::getRTO()
{
	return 2;	// ��ʱ��Ϊ2��
}
