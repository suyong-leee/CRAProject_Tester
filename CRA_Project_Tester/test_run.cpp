#include "gmock/gmock.h"
#include <string>
#include "tester.h"
using namespace std;
using namespace testing;


class MockWriteDriver_ : public Write {
public:
    MOCK_METHOD(void, run, (std::string, std::string), (override));
    MOCK_METHOD(void, write, (std::string address, std::string data), ());
};

class MockReadDriver_ : public Read {
public:
    MOCK_METHOD(void, run, (std::string, std::string), (override));
    MOCK_METHOD(void, read, (std::string address), ());
};

TEST(tester, FullWriteAndReadCompareTest)
{
    MockWriteDriver_ mockWrite;
    MockReadDriver_ mockRead;

    EXPECT_CALL(mockWrite, run(_, _)).Times(100);
    EXPECT_CALL(mockRead, run(_, _)).Times(100);

    SSDTest_FullWriteAndReadCompare ssd(&mockWrite, &mockRead);
    ssd.run("","");
}
