#include "Msg.h"

using namespace std;

// 默认构造
RealMsg::RealMsg()
{}

// 根据消息体，构造消息
RealMsg::RealMsg(const char* data, int len)
{
	m_head.seq = 0;
	m_head.timestamp = (unsigned int)time(NULL);
	m_head.rto = 2;
	m_head.ACK = false;
	m_data = string(data, len);
}

// 从序列化的消息结构中，构造消息
RealMsg::RealMsg(string buf)
{
	Deserialize(buf);
}

// 生成确认包
RealMsg* RealMsg::GetACK()
{
	RealMsg* ackMsg = new RealMsg();
	ackMsg->m_head = m_head;
	ackMsg->m_head.ACK = true;
	return ackMsg;
}

// 序列化消息
string RealMsg::Serialize()
{
	string buf((char*)&m_head, sizeof(Head));
	buf += m_data;
	return buf;
}

// 反序列化消息
void RealMsg::Deserialize(string buf)
{
	m_head = *(Head*)(buf.c_str());
	m_data = buf.substr(sizeof(Head));
}
