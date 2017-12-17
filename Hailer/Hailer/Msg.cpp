#include "Msg.h"

using namespace std;

// Ĭ�Ϲ���
RealMsg::RealMsg()
{}

// ������Ϣ�壬������Ϣ
RealMsg::RealMsg(const char* data, int len)
{
	m_head.seq = 0;
	m_head.timestamp = (unsigned int)time(NULL);
	m_head.rto = 2;
	m_head.ACK = false;
	m_data = string(data, len);
}

// �����л�����Ϣ�ṹ�У�������Ϣ
RealMsg::RealMsg(string buf)
{
	Deserialize(buf);
}

// ����ȷ�ϰ�
RealMsg* RealMsg::GetACK()
{
	RealMsg* ackMsg = new RealMsg();
	ackMsg->m_head = m_head;
	ackMsg->m_head.ACK = true;
	return ackMsg;
}

// ���л���Ϣ
string RealMsg::Serialize()
{
	string buf((char*)&m_head, sizeof(Head));
	buf += m_data;
	return buf;
}

// �����л���Ϣ
void RealMsg::Deserialize(string buf)
{
	m_head = *(Head*)(buf.c_str());
	m_data = buf.substr(sizeof(Head));
}
