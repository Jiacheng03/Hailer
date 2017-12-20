#include "Hailer.h"
#include <thread>
#include <iostream>

// 初始化数据发送模块
Hailer::Hailer(unsigned short myport, const char* peerip, unsigned short peerPort)
:m_talker(myport, peerip, peerPort)
{
}

// 开始服务
void Hailer::Start()
{	
	// 持续获取来自对端的数据
	t_bkRecv.SetRun(bind(&Hailer::thread_recv, this), 0);

	// 将重传队列的数据发出
	t_bkSend.SetRun(bind(&Hailer::thread_resend, this), 0);

	t_bkRecv.Start();
	t_bkSend.Start();

	// 从消息队列取出消息，打印在控制台
	t_recv.SetRun([this](){
		string buf = Recv();
		if (!buf.empty())		
			printf("from peer: %s\n", buf.c_str());			
	}, 0);
	t_recv.Start();

	// 从控制台读取消息，放入重传队列
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

// 结束整个进程
void Hailer::Stop()
{
	// 终止线程
	t_bkRecv.Stop();
	t_bkSend.Stop();

	t_send.Stop();
	t_recv.Stop();

	// 解放消息队列占用的锁
	m_sendList.Stop();
	m_recvList.Stop();	

	this_thread::sleep_for(chrono::milliseconds(300));
	exit(0);
}

//发送数据，即将数据加入消息队列
void Hailer::Send(const char* buf, int len)
{
	Msg msg = make_shared<RealMsg>(buf, len, 0);	
	m_sendList.Push_front(msg);	
}

// 接收数据，即从消息队列取数据
string Hailer::Recv()
{
	Msg msg = m_recvList.Pop();
	return msg->m_data;
}

// 负责从端口发送数据
void Hailer::thread_resend()
{		
	// 发送数据，直到消息队列为空
	while (m_sendList.CheckTime((unsigned int)time(NULL)))
	{
		Msg msg = m_sendList.Pop();
		string buf = msg->Serialize();
		m_talker.Send(buf.c_str(), buf.size());

		msg->m_head.timestamp = (unsigned int)time(NULL);	// 更新时间戳
		msg->m_head.rto = getRTO();			// 暂时定为常数2秒
		m_sendList.InsertByOrder(msg);
	}
}

// 负责监听端口，持续调用m_talker接收数据
void Hailer::thread_recv()
{
	char buf[1024] = { 0 };
	int size = m_talker.Recv(buf, sizeof(buf));
	if (size == -ECONNRESET)		// 对方未监听端口
	{
		printf("%s\n", "peer is not online.");
		return;
	}
	else if (size <= 0)		// 其它错误
	{		
		return;
	}

	Msg msg = make_shared<RealMsg>(string(buf, size));

	if (msg->m_head.timestamp + msg->m_head.rto < time(NULL))	// 若已超时，则忽略			
	{
		return;
	}
	else if (msg->m_head.ACK)		// 若收到的是ack
	{
		m_sendList.Erase(msg->m_head.seq);
	}
	else  // 收到的是数据包
	{
		m_recvList.Push_back(msg);

		// 发送ACK			
		Msg ackMsg( msg->GetACK());
		string ackbuf = ackMsg->Serialize();
		
		m_talker.Send(ackbuf.c_str(), ackbuf.size());
	}
}

// 计算rto，单位暂定为秒
unsigned int Hailer::getRTO()
{
	return 2;	// 暂时定为2秒
}
