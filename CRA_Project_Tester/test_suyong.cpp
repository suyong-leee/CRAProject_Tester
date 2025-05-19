#include "gmock/gmock.h"
#include <string>
#include "tester.h"
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

TEST(mainfunction,exit)
{
	MockTestRun runner;
	EXPECT_CALL(runner, getInput())
		.Times(1)
		.WillRepeatedly(Return("exit"));
	
	runner.RunCommand();
}

TEST(mainfunction, read)
{
	MockTestRun runner;
	MockOperator mockOperator;
	EXPECT_CALL(runner, getInput())
		.Times(2)
		.WillOnce(Return("read"))
	    .WillOnce(Return("0xa"));

	EXPECT_CALL(runner, getOperator(0))
		.Times(1)
		.WillRepeatedly(Return(&mockOperator));

	EXPECT_CALL(mockOperator, run(_, _))
		.Times(1);

	runner.RunCommand();
}

TEST(mainfunction, help)
{
	MockTestRun runner;
	MockOperator mockOperator;
	EXPECT_CALL(runner, getInput())
		.Times(1)
		.WillRepeatedly(Return("help"));

	EXPECT_CALL(runner, getOperator(1))
		.Times(1)
		.WillRepeatedly(Return(&mockOperator));

	EXPECT_CALL(mockOperator, run(_, _))
		.Times(1);

	runner.RunCommand();
}

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
	EXPECT_EQ(expectResult, output);
}