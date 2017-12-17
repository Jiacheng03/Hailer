#include <iostream>
#include "Hailer.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		cout << "<usage>: Hailer.exe myPort peerIp peerPort" << endl;
		cout << "<usage>: 也可以直接使用Debug或Release目录下的start.bat，启动程序." << endl;
		system("pause");
	}
	else
	{
		Hailer hailer(atoi(argv[1]), argv[2], atoi(argv[3]));
		hailer.Start();
	}
	
	return 0;
}
