#pragma once

#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>

using namespace std;

#pragma pack(push,1)
struct Msg
{	
	unsigned int seq;
	unsigned int time;	// 发送时的时间戳
	unsigned int rto;	// 重传超时
	unsigned int size;
	bool ACK;
	char data[0];
};
#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////
class MsgList
{
public:
	MsgList();

	// 将新消息添加到队列头部，并赋予序列号
	void Push_front(shared_ptr<Msg>& pMsg);

	// 将新消息添加到队列尾部
	void Push_back(shared_ptr<Msg>& pMsg);

	// 将队首消息取出，并返回
	shared_ptr<Msg> Pop();

	// 判断队首消息是否已超时
	bool CheckTime(unsigned int now);

	// 将队首消息重传一次后，调整位置，更新时间戳
	//void Refresh();

	// 按照超时时间从小到大的顺序插入
	void InsertByOrder(shared_ptr<Msg>& pMsg);

	// 根据序列号，移除一个消息
	void Erase(int seq);	

	// 当程序终止时，打扫战场
	void Stop();

private:
	list<shared_ptr<Msg>> m_list;	
	unsigned int m_maxSeq;

	mutex m_mutex;
	condition_variable m_cv;
};
