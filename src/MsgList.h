#pragma once

#include "Msg.h"

#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>

#include <time.h>
#include <string>

using namespace std;

class MsgList
{
public:
	// 初始化最大序列号
	MsgList();

	// 将新消息添加到队列头部，并赋予序列号
	void Push_front(Msg msg);

	// 将新消息添加到队列尾部
	void Push_back(Msg msg);

	// 将队首消息取出，并返回
	Msg Pop();

	// 判断队首消息是否已超时
	bool CheckTime(unsigned int now);

	// 将队首消息重传一次后，调整位置，更新时间戳
	//void Refresh();

	// 按照超时时间从小到大的顺序插入
	void InsertByOrder(Msg msg);

	// 根据序列号，移除一个消息
	void Erase(int seq);	

	// 停止使用
	void Stop();

private:
	list<Msg> m_list;
	unsigned int m_maxSeq;

	mutex m_mutex;
	condition_variable m_cv;
};
