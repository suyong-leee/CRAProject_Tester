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

class SSDTestFixture : public ::testing::Test {
protected:
    MockWriteDriver_ mockWrite;
    MockReadDriver_ mockRead;
    SSDTest_FullWriteAndReadCompare* ssd;

    void SetUp() override {
        ssd = new SSDTest_FullWriteAndReadCompare(&mockWrite, &mockRead);
    }

    void TearDown() override {
        delete ssd;
    }
};

TEST_F(SSDTestFixture, FullWriteAndReadCompareFailTest) {
    EXPECT_CALL(mockWrite, run(_, _)).Times(::testing::AnyNumber());
    EXPECT_CALL(mockRead, read(_))
        .Times(::testing::AnyNumber())
        .WillOnce(::testing::Return("0x12341234"));

    EXPECT_THROW(ssd->run("", ""), exception);
}

TEST_F(SSDTestFixture, FullWriteAndReadCompareTest) {
    EXPECT_CALL(mockWrite, run(_, "0x11111111")).Times(::testing::AnyNumber());
    EXPECT_CALL(mockRead, read(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return("0x11111111"));

    ssd->run("", "");
}