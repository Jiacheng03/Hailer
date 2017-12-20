#pragma once

#include <string>
#include <time.h>
#include <memory>

struct Head;	// ��Ϣͷ
struct RealMsg;	// ��Ϣ�ṹ
typedef std::shared_ptr<RealMsg> Msg;	// ������Ϣ�ṹ������ָ�룬�Ƽ�ʹ��

#pragma pack(1)
// ��Ϣͷ
struct Head
{	
	unsigned int seq;
	unsigned int timestamp;	// ����ʱ��ʱ���
	unsigned int rto;	// �ش���ʱ	
	unsigned int size;
	bool ACK;	// �Ƿ���ack��Ϣ
};

struct RealMsg
{
	Head m_head;	// ��Ϣͷ
	std::string m_data;	// ��Ϣ�壨��������ʵ�壩

	// Ĭ�Ϲ��캯��
	RealMsg();

	// ������Ϣ�壬������Ϣ
	RealMsg(const char* data, int len, unsigned int rto=2);

	// �����л�����Ϣ�ṹ�У�������Ϣ
	RealMsg(std::string buf);

	// ����ȷ�ϰ�
	RealMsg* GetACK();

	// ���л���Ϣ
	std::string Serialize();

	// �����л���Ϣ
	void Deserialize(std::string buf);
};
#pragma pack()
