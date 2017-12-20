#include <iostream>
#include "Talker.h"

using namespace std;

// ��ʼ��WSADATA���󶨱��ض˿ڣ�����UDP�����ӡ���
Talker::Talker(unsigned short myport, const char* peerip, unsigned short peerPort)
{
	init();
	OpenPort(myport);
	AimAt(peerip, peerPort);
}

// ����WSADATA���׽��֣�
Talker::~Talker()
{
#ifdef _MSC_VER
	closesocket(m_sockfd);	
	WSACleanup();
#else
	close(m_sockfd);
#endif
}

// ��ʼ��WSADATA��
void Talker::init()
{	
#ifdef _MSC_VER
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Could not load winsock\n";
		exit(GetErrno());
	}
#endif
}

// �׽��ְ󶨱��ض˿ڣ�
void Talker::OpenPort(unsigned short port)
{
	sockaddr_in servaddr = { 0 };
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	
	if ((m_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		cerr << "socket fail" << endl;
		exit(GetErrno());
	}

	// ���׽�������Ϊ������
	SetNonBlock(m_sockfd);

	if (bind(m_sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) != 0)
	{
		cerr << "bind fail\n";
		exit(GetErrno());
	}
}

// ����UDP�����ӡ���
void Talker::AimAt(const char* ip, unsigned short port)
{
	sockaddr_in servaddr = { 0 };
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	if (connect(m_sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		cerr << "connect fail\n";
		exit(GetErrno());
	}
}

// ��������
int Talker::Send(const char* buf, int len)
{	
	int size = 0;
	if ((size = send(m_sockfd, buf, len, 0)) < 0)
	{	
		cerr << "Talker::Send failed\n";	
		return -1 * GetErrno();
	}		
	return size;
}

// ��������
int Talker::Recv(char * buf, int len)
{
	int size = 0;
	if ((size = recv(m_sockfd, buf, len, 0)) < 0)
	{	
		return -1 * GetErrno();
	}	
	return size;
}

// ��ȡ������
int Talker::GetErrno()
{
#ifdef _MSC_VER
	return WSAGetLastError();
#else
	return errno;
#endif
}

// ����socketΪ����������
void Talker::SetNonBlock(SOCKET sockfd)
{
#ifdef _MSC_VER
		u_long lMode = 1;
		ioctlsocket(sockfd, FIONBIO, &lMode);
#else
		int flags = fcntl(sockfd, F_GETFL, 0);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
}
