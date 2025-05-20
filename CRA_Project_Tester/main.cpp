#include "gmock/gmock.h"
#include <iostream>
#include "Logger.h"
#include "testRunner.h"

#define __DEBUG__ (1)

using namespace std;
using namespace testing;
Logger loggerInstance;

#if (__DEBUG__ == 1)
int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
#else
int main()
{
    TestRun testrun;
    while (testrun.RunCommand() == true)
    {
    }
    return 0;
}
#endif
