#include "gmock/gmock.h"
#include <string>
#include <iostream>
#include "tester.h"

using namespace testing;
using namespace std;



class MockReadDriver : public Read {
public:
    MOCK_METHOD(string, Read, (string cmd1), ());

};


TEST(SSDTEST, ReadNone) {

    MockReadDriver mockDriver;
    EXPECT_CALL(mockDriver, Read("3")).WillRepeatedly(Return("helloWorld"));
	//cout << mockDriver.Read("3") << endl;
	EXPECT_EQ(mockDriver.Read("3"), "helloWorld");
}

TEST(SSDTEST, LBAinvalid) {

    MockReadDriver mockDriver;
    EXPECT_NO_THROW({
    bool result = mockDriver.checkCMD("12");
        EXPECT_TRUE(result);
    });
}

TEST(SSDTEST, LBAinvalid2) {

    MockReadDriver mockDriver;
    EXPECT_THROW(mockDriver.checkCMD("123"), std::invalid_argument);
}

TEST(SSDTEST, LBAinvalid3) {
    MockReadDriver mockDriver;
    EXPECT_THROW(mockDriver.checkCMD("1A"), std::invalid_argument);
}

