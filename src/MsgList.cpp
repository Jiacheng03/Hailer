#include "MsgList.h"
#include <time.h>

// 初始化最大序列号
MsgList::MsgList()
{
	m_maxSeq = 0;
}

// 停止使用
void MsgList::Stop()
{
	Msg msg = make_shared<RealMsg>();
	Push_front(msg);		// 解放阻塞的Pop
}

// 将新消息添加到队列头部，并赋予序列号
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

// 将新消息添加到队列尾部
void MsgList::Push_back(Msg msg)
{
	unique_lock<mutex> lck(m_mutex);
	m_list.push_back(msg);
	if (m_list.size() == 1)
	{
		m_cv.notify_one();
	}	
}

// 将队首消息取出，并返回; 当列表为空时，阻塞;
Msg MsgList::Pop()
{
	Msg listHead;
	unique_lock<mutex> lck(m_mutex);

	m_cv.wait(lck, [this](){return !m_list.empty(); });
	listHead = *(m_list.begin());
	m_list.pop_front();	

	return listHead;
}

// 判断队首消息是否已超时
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

// 按照超时时间从小到大的顺序插入
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

// 根据序列号，移除一个消息
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
