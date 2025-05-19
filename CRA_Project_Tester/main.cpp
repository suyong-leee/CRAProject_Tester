#include "gmock/gmock.h"
#include <iostream>

#include "tester.h"

#define __DEBUG__ (1)

using namespace std;
using namespace testing;
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
        string command, param1, param2;
        cin >> command;
        cout << command << endl;
        if (command == "read") {
            cin >> param1;
            operation = new Read;
        }
        else if (command == "help")
	    operation = new Help;	
	else if (command == "exit")
	    break;
        else
            continue;
        operation->run(param1, param2);
    }
    return 0;
}
#endif
