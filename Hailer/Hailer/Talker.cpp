#include <iostream>
#include "Talker.h"

using namespace std;

//// 获取标准输入，再发送给对端；它以线程函数的形式被调用；
//DWORD WINAPI ThreadFunc(LPVOID pParam)
//{	
//	while (true)
//	{
//		char buf[MAXBYTE] = { 0 };
//		cin.getline(buf, MAXBYTE);
//		((Talker*)pParam)->Send(buf, strlen(buf) + 1);
//	}
//	return 0;
//}


// 初始化WSADATA，绑定本地端口，建立UDP“连接”；
Talker::Talker(unsigned short myport, const char* peerip, unsigned short peerPort)
{
	init();
	OpenPort(myport);
	AimAt(peerip, peerPort);
}


// 清理WSADATA和套接字；
Talker::~Talker()
{
	closesocket(m_sockfd);	
	WSACleanup();
}


// 初始化WSADATA；
void Talker::init()
{	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Could not load winsock" << endl;
		exit(1);
	}
}


// 套接字绑定本地端口；
void Talker::OpenPort(unsigned short port)
{
	sockaddr_in servaddr = { 0 };
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	
	if ((m_sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cerr << "socket fail" << endl;
		exit(WSAGetLastError());
	}
	u_long lMode = 1;
	ioctlsocket(m_sockfd, FIONBIO, &lMode);

	if (bind(m_sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) != 0)
	{
		cerr << "bind fail" << endl;
		exit(WSAGetLastError());
	}
}


// 建立UDP“连接”；
void Talker::AimAt(const char* ip, unsigned short port)
{
	sockaddr_in servaddr = { 0 };
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	if (connect(m_sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		cerr << "connect fail" << endl;
		exit(WSAGetLastError());
	}
}


//// 创建线程，获取标准输入，发送给对端；
//// 接收来自对端的消息，并打印；
//void Talker::Start()
//{
//	cout << "talking ..." << endl;
//	HANDLE handle = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
//
//	while (true)
//	{
//		char buf[1024] = { 0 };
//		int size = Recv(buf, sizeof(buf));		
//		if (size > 0)
//		{
//			printf("from peer: %s\n", buf);
//		}
//		else if (size == -WSAECONNRESET)
//		{
//			printf("%s\n", "peer is not online.");
//		}
//		else
//		{
//			Sleep(1000);
//		}
//	}
//}


int Talker::Send(const char* buf, int len)
{	
	int size = 0;
	if ((size = send(m_sockfd, buf, len, 0)) < 0)
	{		
		return WSAGetLastError();
	}		
	return size;
}

int Talker::Recv(char * buf, int len)  // **
{
	int size = 0;
	if ((size = recv(m_sockfd, buf, len, 0)) < 0)
	{	
		return -1*WSAGetLastError();
	}		
	return size;
}

