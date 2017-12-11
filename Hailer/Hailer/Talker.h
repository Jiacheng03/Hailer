#pragma once

#include <windows.h>

#pragma comment(lib, "ws2_32")

class Talker
{
public:
	// ��ʼ��WSADATA���󶨱��ض˿ڣ�����UDP�����ӡ���
	Talker(unsigned short myport, const char* peerip, unsigned short peerPort);

	// ����WSADATA���׽��֣�
	~Talker();

	//// �����̣߳���ȡ��׼���룬���͸��Զˣ�
	//// �������ԶԶ˵���Ϣ������ӡ��
	//void Start();

	int Send(const char* buf, int len);
	int Recv(char * buf, int len);

private:
	SOCKET m_sockfd;

	// ��ʼ��WSADATA��
	void init();

	// �׽��ְ󶨱��ض˿ڣ�
	void OpenPort(unsigned short port);

	// ����UDP�����ӡ���
	void AimAt(const char* ip, unsigned short port);	
};