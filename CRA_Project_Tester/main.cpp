#include "gmock/gmock.h"
#include <iostream>
#include "Logger.h"
#include "testRunner.h"
#include "testScript.h"
#include <string>

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
int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        string file_name(argv[1]);
        if (file_name.find(".txt") != string::npos)
            (new SSDTest_FullScenario)->run(file_name, "");

        return 0;
    }

    TestRun testrun;
    while (testrun.RunCommand() == true)
    {
    }
    return 0;
}
#endif
