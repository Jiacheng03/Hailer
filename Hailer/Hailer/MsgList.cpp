#include "MsgList.h"
#include <time.h>

MsgList::MsgList()
{
	m_maxSeq = 0;
}

// 当程序终止时，打扫战场
void MsgList::Stop()
{
	shared_ptr<Msg> defMsg(new Msg);
	defMsg->size = 0;
	Push_front(defMsg);		// 解放阻塞的Pop
}

// 将新消息添加到队列头部，并赋予序列号
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

// 将新消息添加到队列尾部
void MsgList::Push_back(shared_ptr<Msg>& pMsg)
{
	unique_lock<mutex> lck(m_mutex);
	m_list.push_back(pMsg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// 将队首消息取出，并返回; 当列表为空时，阻塞;
shared_ptr<Msg> MsgList::Pop()
{
	shared_ptr<Msg> pHead;
	unique_lock<mutex> lck(m_mutex);

	m_cv.wait(lck, [this](){return !m_list.empty(); });
	pHead = *(m_list.begin());
	m_list.pop_front();	

	return pHead;
}

// 判断队首消息是否已超时
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

// 按照超时时间从小到大的顺序插入
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

// 根据序列号，移除一个消息
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
