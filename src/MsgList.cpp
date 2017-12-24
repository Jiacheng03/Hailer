#include "MsgList.h"
#include <time.h>

// ��ʼ��������к�
MsgList::MsgList()
{
	m_maxSeq = 0;
}

// ֹͣʹ��
void MsgList::Stop()
{
	Msg msg = make_shared<RealMsg>();
	Push_front(msg);		// ���������Pop
}

// ������Ϣ��ӵ�����ͷ�������������к�
void MsgList::Push_front(Msg msg)
{
	msg->m_head.seq = m_maxSeq++;
	unique_lock<mutex> lck(m_mutex);
	m_list.push_front(msg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// ������Ϣ��ӵ�����β��
void MsgList::Push_back(Msg msg)
{
	unique_lock<mutex> lck(m_mutex);
	m_list.push_back(msg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// ��������Ϣȡ����������; ���б�Ϊ��ʱ������;
Msg MsgList::Pop()
{
	Msg listHead;
	unique_lock<mutex> lck(m_mutex);

	m_cv.wait(lck, [this](){return !m_list.empty(); });
	listHead = *(m_list.begin());
	m_list.pop_front();	

	return listHead;
}

// �ж϶�����Ϣ�Ƿ��ѳ�ʱ
bool MsgList::CheckTime(unsigned int now)
{
	bool res = false;
	unique_lock<mutex> lck(m_mutex);
	if (!m_list.empty())
	{
		Msg msg = *(m_list.begin());	
		res = (msg->m_head.timestamp + msg->m_head.rto) <= now;
	}		
	return res;
}

// ���ճ�ʱʱ���С�����˳�����
void MsgList::InsertByOrder(Msg msg)
{
	unique_lock<mutex> lck(m_mutex);

	list<Msg>::iterator it = m_list.begin();
	for (; it != m_list.end() &&
		(msg->m_head.timestamp + msg->m_head.rto) > ((*it)->m_head.timestamp + (*it)->m_head.rto);
		it++);

	m_list.insert(it, msg);

	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// �������кţ��Ƴ�һ����Ϣ
void MsgList::Erase(unsigned int seq)
{
	unique_lock<mutex> lck(m_mutex);
	for (list<Msg>::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it)->m_head.seq == seq)
		{
			m_list.erase(it);
			break;
		}
	}	
}
