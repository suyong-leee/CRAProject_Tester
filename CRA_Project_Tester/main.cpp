#include "gmock/gmock.h"
#include <iostream>

#include "tester.h"

#define __DEBUG__ (1)

using namespace std;
#if (__DEBUG__ == 1)
int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main()
{
	ITestOperation* operation = nullptr;
	while (1)
	{
		string command;
		cin >> command;
		if (command == "read")
			operation = new Read;
		else if (command == "help")
			operation = new Help;
		else
			continue;
		operation->run(command);
	}
	return 0;
}
#endif