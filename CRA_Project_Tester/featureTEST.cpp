#include "gmock/gmock.h"
#include <string>
#include <iostream>
#include "tester.h"

using namespace testing;
using namespace std;



class MockReadDriver : public Read {
public:
    MOCK_METHOD(string, Read, (string cmd1), ());
    MOCK_METHOD(void, run, (std::string, std::string), (override));
};



TEST(SSDTEST, ReadNone) {
    
    MockReadDriver mockDriver;
    EXPECT_CALL(mockDriver, Read("3")).WillRepeatedly(Return("helloWorld"));
    //cout << mockDriver.Read("3") << endl;
    EXPECT_EQ(mockDriver.Read("3"), "helloWorld");
}