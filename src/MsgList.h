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
	// ��ʼ��������к�
	MsgList();

	// ������Ϣ��ӵ�����ͷ�������������к�
	void Push_front(Msg msg);

	// ������Ϣ��ӵ�����β��
	void Push_back(Msg msg);

	// ��������Ϣȡ����������
	Msg Pop();

	// �ж϶�����Ϣ�Ƿ��ѳ�ʱ
	bool CheckTime(unsigned int now);

	// ��������Ϣ�ش�һ�κ󣬵���λ�ã�����ʱ���
	//void Refresh();

	// ���ճ�ʱʱ���С�����˳�����
	void InsertByOrder(Msg msg);

	// �������кţ��Ƴ�һ����Ϣ
	void Erase(int seq);	

	// ֹͣʹ��
	void Stop();

private:
	list<Msg> m_list;
	unsigned int m_maxSeq;

	mutex m_mutex;
	condition_variable m_cv;
};
