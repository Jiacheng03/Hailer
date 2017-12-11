#pragma once

#include <windows.h>

#pragma comment(lib, "ws2_32")

class Talker
{
public:
	// 初始化WSADATA，绑定本地端口，建立UDP“连接”；
	Talker(unsigned short myport, const char* peerip, unsigned short peerPort);

	// 清理WSADATA和套接字；
	~Talker();

	//// 创建线程，获取标准输入，发送给对端；
	//// 接收来自对端的消息，并打印；
	//void Start();

	int Send(const char* buf, int len);
	int Recv(char * buf, int len);

private:
	SOCKET m_sockfd;

	// 初始化WSADATA；
	void init();

	// 套接字绑定本地端口；
	void OpenPort(unsigned short port);

	// 建立UDP“连接”；
	void AimAt(const char* ip, unsigned short port);	
};