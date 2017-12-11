#include <iostream>
#include "Hailer.h"
//#include "MsgList.h"

using namespace std;

int main(int argc, char** argv)
{
	/*Hailer hailer(33333, "127.0.0.1", 44444);
	hailer.Start();*/

	if (argc < 4)
	{
		cout << "<usage>: Hailer.exe myPort peerIp peerPort" << endl;
	}
	else
	{
		Hailer hailer(atoi(argv[1]), argv[2], atoi(argv[3]));
		hailer.Start();
	}
	
	//system("pause");
	return 0;
}
