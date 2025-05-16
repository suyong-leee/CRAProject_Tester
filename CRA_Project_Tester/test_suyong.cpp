#include "gmock/gmock.h"
#include <string>
#include "tester.h"
using namespace std;
TEST(Help, operationtest)
{
	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

	Help help;
	
	std::string expectResult;
	expectResult = "Team Approve : ��ƶ� �̵��� �̼��� �̼��� �̿��� �ѻ��\n";
	expectResult.append("How to use CMD\n");
	expectResult.append("read (address) : (address)�� �����͸� ����\n");
	expectResult.append("write (address) (data) : (address)�� (data)�� ��\n");
	expectResult.append("fullread : ��� �����͸� ����\n");
	expectResult.append("fullwrite (data) : ��� �ּҿ� (data)�� ��\n");
	expectResult.append("exit : ����\n");
	expectResult.append("help : ����\n");
	
	help.run();
	std::cout.rdbuf(old);
	std::string output = buffer.str();
	EXPECT_EQ(expectResult, output);
}