#include "gmock/gmock.h"
#include <string>
#include "tester.h"
using namespace std;
using namespace testing;


class MockWriteDriver : public Write {
public:
    MOCK_METHOD(void, run, (std::string, std::string), (override));
    MOCK_METHOD(void, write, (std::string address, std::string data), ());
};

class MockReadDriver : public Read {
public:
    MOCK_METHOD(void, run, (std::string, std::string), (override));
    MOCK_METHOD(std::string, read, (std::string address), ());
};

TEST(tester, runtest_InvalidInput)
{
    std::streambuf* originalCoutBuffer = std::cout.rdbuf();

    std::stringstream capturedOutput;
    std::cout.rdbuf(capturedOutput.rdbuf()); 

    Write write;
    Read read;
    SSDTest ssd(&write, &read);
    ssd.run("1");
    std::cout.rdbuf(originalCoutBuffer);

    std::string expectedOutput = "input error\n";
    EXPECT_EQ(capturedOutput.str(), expectedOutput);

}


TEST(tester, runtest_validInput)
{
    std::streambuf* originalCoutBuffer = std::cout.rdbuf();

    std::stringstream capturedOutput;
    std::cout.rdbuf(capturedOutput.rdbuf());

    Write write;
    Read read;
    SSDTest ssd(&write, &read);
    ssd.run("1_");
    std::cout.rdbuf(originalCoutBuffer);

    std::string expectedOutput = "test1\n";
    EXPECT_EQ(capturedOutput.str(), expectedOutput);

}

