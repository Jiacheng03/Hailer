#include <iostream>
#include "Hailer.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		cerr << "<usage>: Hailer.exe myPort peerIp peerPort\n";
		cerr << "<usage>: Ҳ����ֱ��ʹ��Debug��ReleaseĿ¼�µ�start.bat����������.\n";
		system("pause");
	}
	else
	{
		Hailer hailer(atoi(argv[1]), argv[2], atoi(argv[3]));
		hailer.Start();
	}
	
	return 0;
}
