#pragma once

#ifdef _MSC_VER

#include <windows.h>
#pragma comment(lib, "ws2_32")

#else

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <errno.h>

#include <unistd.h>
#include <arpa/inet.h>

#endif


#ifdef _MSC_VER
#define ECONNRESET WSAECONNRESET
#else
typedef int SOCKET;
#define	INVALID_SOCKET	(-1)
#endif

class Talker
{
public:
	// ��ʼ��WSADATA���󶨱��ض˿ڣ�����UDP�����ӡ���
	Talker(unsigned short myport, const char* peerip, unsigned short peerPort);

	// ����WSADATA���׽��֣�
	~Talker();

	// ��������
	int Send(const char* buf, int len);

	// ��������
	int Recv(char * buf, int len);

	// ��ȡ������
	int GetErrno();

private:
	SOCKET m_sockfd;

	// ��ʼ��WSADATA��
	void init();

	// �׽��ְ󶨱��ض˿ڣ�
	void OpenPort(unsigned short port);

	// ����UDP�����ӡ���
	void AimAt(const char* ip, unsigned short port);

	// ����socketΪ����������
	void SetNonBlock(SOCKET sockfd);
};