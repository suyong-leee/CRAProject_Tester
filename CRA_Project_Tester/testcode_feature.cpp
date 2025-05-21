#include "gmock/gmock.h"
#include <string>
#include "testRunner.h"
using namespace std;
using namespace testing;

class MockOperator : public ITestOperation
{
public:
    MOCK_METHOD(void, run, (string,string), (override));
};

class MockTestRun : public TestRun
{
public:
    MOCK_METHOD(string, getInput, (), (override));
    MOCK_METHOD(ITestOperation*, getOperator, (int), (override));
};

class MockWriter : public Write
{
public:
    MOCK_METHOD(void, callSSD, (string, string), (override));
};

class MockReader : public Read {
public:
    MOCK_METHOD(string, read, (string cmd1), (override));

    MOCK_METHOD(void, run, (string, string), (override));

};

class MockFullWriter : public FullWrite
{
public:
    MOCK_METHOD(void, callSSD, (string, string), (override));
};

class MockFullRead : public FullRead {
public:
    MOCK_METHOD(string, read, (string address), (override));
};

//Invalid Test
TEST(mainfunction, InvalidCommand)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillOnce(Return("full"));


    EXPECT_EQ(true, runner.RunCommand());
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    std::string expectResult = "INVALID COMMAND\n";
    EXPECT_THAT(output, testing::HasSubstr(expectResult));
}

TEST(Testwrite, InvalidWriteAddress)
{
    Write writer;
    EXPECT_THROW(writer.checkAddress("101"), std::invalid_argument);
}

TEST(Testwrite, InvalidWriteData1)
{
    Write writer;
    EXPECT_THROW(writer.checkData("0x000G000a"), std::invalid_argument);
}

TEST(Testwrite, InvalidWriteData2)
{
    Write writer;
    EXPECT_THROW(writer.checkData("0x111111111"), std::invalid_argument);
}

TEST(Testwrite, InvalidWrite)
{
    MockWriter writer;
    string address = "30";
    string data = "0xAAAAAG";
    EXPECT_CALL(writer, callSSD(address, data))
        .Times(0);

    writer.run(address, data);
}

TEST(Testwrite, InvalidWrite2)
{
    MockWriter writer;
    string address = "";
    string data = "0xAAAAAG";
    EXPECT_CALL(writer, callSSD(address, data))
        .Times(0);

    writer.run(address, data);
}

TEST(SSDTEST, LBAinvalid) {

    MockReader mockDriver;
    EXPECT_NO_THROW({
    bool result = mockDriver.checkLBA("12");
    EXPECT_TRUE(result);
        });
}

TEST(SSDTEST, LBAinvalid2) {

    MockReader mockDriver;
    EXPECT_THROW(mockDriver.checkLBA("123"), std::invalid_argument);
}

TEST(SSDTEST, LBAinvalid3) {
    MockReader mockDriver;
    EXPECT_THROW(mockDriver.checkLBA("1A"), std::invalid_argument);
}

//exit test
TEST(mainfunction, exittest)
{
    MockTestRun runner;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillRepeatedly(Return("exit"));
    
    EXPECT_EQ(false,runner.RunCommand());
}

//read test
TEST(mainfunction, readtest)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(2)
        .WillOnce(Return("read"))
        .WillOnce(Return("0xa"));

    EXPECT_CALL(runner, getOperator(TestRun::OPERATOR_READ))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true,runner.RunCommand());
}

TEST(mainfunction, fullread)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillOnce(Return("fullread"));

    EXPECT_CALL(runner, getOperator(TestRun::OPERATOR_FULLREAD))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true, runner.RunCommand());
}

TEST(FullReadTest, CallsRead100Times) {
    MockFullRead mock;

    EXPECT_CALL(mock, read(_))
        .Times(Exactly(100))
        .WillRepeatedly(Return("mocked_data"));

    mock.run();
}

TEST(SSDTEST, ReadNone) {

    MockReader mockDriver;
    EXPECT_CALL(mockDriver, read("3")).WillRepeatedly(Return("helloWorld"));

    EXPECT_EQ(mockDriver.read("3"), "helloWorld");
}

//help test
TEST(mainfunction, help1)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillRepeatedly(Return("help"));

    EXPECT_CALL(runner, getOperator(TestRun::OPERATOR_HELP))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true,runner.RunCommand());
}

TEST(mainfunction, help2)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    Help help;

    std::string expectResult;
    expectResult = "Team Approve : 고아라 이동건 이서영 이수용 이용한 한상민\n";
    expectResult.append("How to use CMD\n");
    expectResult.append("read (address) : (address)의 데이터를 읽음\n");
    expectResult.append("write (address) (data) : (address)에 (data)를 씀\n");
    expectResult.append("fullread : 모든 데이터를 읽음\n");
    expectResult.append("fullwrite (data) : 모든 주소에 (data)를 씀\n");
    expectResult.append("exit : 종료\n");
    expectResult.append("help : 도움말\n");

    help.run();
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    EXPECT_THAT(output, testing::HasSubstr(expectResult));
}

//write test
TEST(mainfunction, writetest)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(3)
        .WillOnce(Return("write"))
        .WillOnce(Return("0xa"))
        .WillOnce(Return("0xa"));

    EXPECT_CALL(runner, getOperator(TestRun::OPERATOR_WRITE))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true, runner.RunCommand());
}

TEST(mainfunction, fullwrite1)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(2)
        .WillOnce(Return("fullwrite"))
        .WillOnce(Return("0xaaaaaaaa"));

    EXPECT_CALL(runner, getOperator(TestRun::OPERATOR_FULLWRITE))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true, runner.RunCommand());
}

TEST(mainfunction, fullwrite2)
{
    MockFullWriter writer;
    string data = "0xAAAAAAAA";
    EXPECT_CALL(writer, callSSD(_, data))
        .Times(100);

    writer.run(data);
}

TEST(Testwrite, normalWrite)
{
    MockWriter writer;
    string address = "30";
    string data = "0xAAAAAAAA";
    EXPECT_CALL(writer, callSSD(address, data))
        .Times(1);

    writer.run(address, data);
}

//scenario test
TEST(mainfunction, scenariocommand1)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillOnce(Return("1_"));
    
    EXPECT_CALL(runner, getOperator(TestRun::SCENARIO_1))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true, runner.RunCommand());
}

TEST(mainfunction, scenariocommand2)
{
    MockTestRun runner;
    MockOperator mockOperator;
    EXPECT_CALL(runner, getInput())
        .Times(1)
        .WillOnce(Return("1_FullWriteAndReadCompare"));

    EXPECT_CALL(runner, getOperator(TestRun::SCENARIO_1))
        .Times(1)
        .WillRepeatedly(Return(&mockOperator));

    EXPECT_CALL(mockOperator, run(_, _))
        .Times(1);

    EXPECT_EQ(true, runner.RunCommand());
}
<<<<<<< HEAD:CRA_Project_Tester/test_suyong.cpp


TEST(Help, operationtest)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    Help help;
    
    std::string expectResult;
    expectResult = "Team Approve : 고아라 이동건 이서영 이수용 이용한 한상민\n";
    expectResult.append("How to use CMD\n");
    expectResult.append("read (address) : (address)의 데이터를 읽음\n");
    expectResult.append("write (address) (data) : (address)에 (data)를 씀\n");
    expectResult.append("fullread : 모든 데이터를 읽음\n");
    expectResult.append("fullwrite (data) : 모든 주소에 (data)를 씀\n");
    expectResult.append("exit : 종료\n");
    expectResult.append("help : 도움말\n");
    
    help.run();
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    EXPECT_THAT(output, testing::HasSubstr(expectResult));
}

TEST(Testwrite, InvalidWriteAddress)
{
    Write writer;
    EXPECT_THROW(Validator::checkHexData("101"), std::invalid_argument);
}

TEST(Testwrite, InvalidWriteData1)
{
    Write writer;
    EXPECT_THROW(Validator::checkHexData("0x000G000a"), std::invalid_argument);
}

TEST(Testwrite, InvalidWriteData2)
{
    Write writer;
    EXPECT_THROW(Validator::checkHexData("0x111111111"), std::invalid_argument);
}

class MockWriter : public Write
{
public:
    MOCK_METHOD(void, writeSSD, (string, string), (override));
};

class MockFullWriter : public FullWrite
{
public:
    MOCK_METHOD(void, writeSSD, (string, string), (override));
};

TEST(Testwrite, normalWrite)
{
    MockWriter writer;
    string address = "30";
    string data = "0xAAAAAAAA";
    EXPECT_CALL(writer, writeSSD(address, data))
        .Times(1);

    writer.run(address, data);
}

TEST(Testwrite, InvalidWrite)
{
    MockWriter writer;
    string address = "30";
    string data = "0xAAAAAG";
    EXPECT_CALL(writer, writeSSD(address, data))
        .Times(0);

    writer.run(address, data);
}

TEST(Testwrite, InvalidWrite2)
{
    MockWriter writer;
    string address = "";
    string data = "0xAAAAAG";
    EXPECT_CALL(writer, writeSSD(address, data))
        .Times(0);

    writer.run(address, data);
}

TEST(TestFullWrite, NormalOperation)
{
    MockFullWriter writer;
    string data = "0xAAAAAAAA";
    EXPECT_CALL(writer, writeSSD(_, data))
        .Times(100);

    writer.run(data);
}
=======
>>>>>>> master:CRA_Project_Tester/testcode_feature.cpp
