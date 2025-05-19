#include "gmock/gmock.h"
#include <string>
#include "tester.h"
using namespace std;

class MockWrite : public Write
{	
public:
	MOCK_METHOD(void, write, (string,string), (override));
};

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

TEST(Write, executeNormalWrite)
{
	MockWrite mockWriter;

	EXPECT_CALL(mockWriter, write("0x12", "0x33"))
		.Times(1);

	mockWriter.run("0x12","0x33");
}