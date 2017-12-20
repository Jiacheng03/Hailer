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
	// 初始化WSADATA，绑定本地端口，建立UDP“连接”；
	Talker(unsigned short myport, const char* peerip, unsigned short peerPort);

	// 清理WSADATA和套接字；
	~Talker();

	// 发送数据
	int Send(const char* buf, int len);

	// 接收数据
	int Recv(char * buf, int len);

	// 获取错误码
	int GetErrno();

private:
	SOCKET m_sockfd;

	// 初始化WSADATA；
	void init();

	// 套接字绑定本地端口；
	void OpenPort(unsigned short port);

	// 建立UDP“连接”；
	void AimAt(const char* ip, unsigned short port);

	// 设置socket为非阻塞类型
	void SetNonBlock(SOCKET sockfd);
};