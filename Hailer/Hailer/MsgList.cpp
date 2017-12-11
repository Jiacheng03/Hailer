#include "MsgList.h"
#include <time.h>

MsgList::MsgList()
{
	m_maxSeq = 0;
}

// ��������ֹʱ����ɨս��
void MsgList::Stop()
{
	shared_ptr<Msg> defMsg(new Msg);
	defMsg->size = 0;
	Push_front(defMsg);		// ���������Pop
}

// ������Ϣ��ӵ�����ͷ�������������к�
void MsgList::Push_front(shared_ptr<Msg>& pMsg)
{
	pMsg->seq = m_maxSeq++;
	unique_lock<mutex> lck(m_mutex);
	m_list.push_front(pMsg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// ������Ϣ��ӵ�����β��
void MsgList::Push_back(shared_ptr<Msg>& pMsg)
{
	unique_lock<mutex> lck(m_mutex);
	m_list.push_back(pMsg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// ��������Ϣȡ����������; ���б�Ϊ��ʱ������;
shared_ptr<Msg> MsgList::Pop()
{
	shared_ptr<Msg> pHead;
	unique_lock<mutex> lck(m_mutex);

	m_cv.wait(lck, [this](){return !m_list.empty(); });
	pHead = *(m_list.begin());
	m_list.pop_front();	

	return pHead;
}

// �ж϶�����Ϣ�Ƿ��ѳ�ʱ
bool MsgList::CheckTime(unsigned int now)
{
	bool res = false;
	unique_lock<mutex> lck(m_mutex);
	if (!m_list.empty())
	{
		shared_ptr<Msg> pMsg = *(m_list.begin());
		res = (pMsg->time + pMsg->rto) <= now;
	}		
	return res;
}

// ���ճ�ʱʱ���С�����˳�����
void MsgList::InsertByOrder(shared_ptr<Msg>& pMsg)
{
	unique_lock<mutex> lck(m_mutex);
	list<shared_ptr<Msg>>::iterator it = m_list.begin();
	for (; it != m_list.end(); it++)
	{
		if ((pMsg->time + pMsg->rto) < ((*it)->time + (*it)->rto))
		{
			m_list.insert(it, pMsg);			
			break;
		}
	}
	if (it == m_list.end())
	{
		m_list.insert(it, pMsg);
	}	

	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// �������кţ��Ƴ�һ����Ϣ
void MsgList::Erase(int seq)
{
	unique_lock<mutex> lck(m_mutex);
	for (list<shared_ptr<Msg>>::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it)->seq == seq)
		{
			m_list.erase(it);
			break;
		}
	}	
}
