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
    MOCK_METHOD(string, read, (std::string address), ());
};

TEST(tester, FullWriteAndReadCompareFailTest)
{
    MockWriteDriver_ mockWrite;
    MockReadDriver_ mockRead;

    EXPECT_CALL(mockWrite, run(_, _)).Times(AnyNumber());
    EXPECT_CALL(mockRead, read(_)).Times(AnyNumber()).WillOnce(Return("0x12341234"));

    SSDTest_FullWriteAndReadCompare ssd(&mockWrite, &mockRead);
    
    EXPECT_THROW(ssd.run("", ""), exception);
}

TEST(tester, FullWriteAndReadCompareTest)
{
    MockWriteDriver_ mockWrite;
    MockReadDriver_ mockRead;

    EXPECT_CALL(mockWrite, run(_, _)).Times(AnyNumber());
    EXPECT_CALL(mockRead, read(_)).Times(AnyNumber()).WillRepeatedly(Return("0x11111111"));

    SSDTest_FullWriteAndReadCompare ssd(&mockWrite, &mockRead);

    ssd.run("", "");
}
