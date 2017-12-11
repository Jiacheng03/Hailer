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
	unsigned int time;	// ����ʱ��ʱ���
	unsigned int rto;	// �ش���ʱ
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

	// ������Ϣ��ӵ�����ͷ�������������к�
	void Push_front(shared_ptr<Msg>& pMsg);

	// ������Ϣ��ӵ�����β��
	void Push_back(shared_ptr<Msg>& pMsg);

	// ��������Ϣȡ����������
	shared_ptr<Msg> Pop();

	// �ж϶�����Ϣ�Ƿ��ѳ�ʱ
	bool CheckTime(unsigned int now);

	// ��������Ϣ�ش�һ�κ󣬵���λ�ã�����ʱ���
	//void Refresh();

	// ���ճ�ʱʱ���С�����˳�����
	void InsertByOrder(shared_ptr<Msg>& pMsg);

	// �������кţ��Ƴ�һ����Ϣ
	void Erase(int seq);	

	// ��������ֹʱ����ɨս��
	void Stop();

private:
	list<shared_ptr<Msg>> m_list;	
	unsigned int m_maxSeq;

	mutex m_mutex;
	condition_variable m_cv;
};
