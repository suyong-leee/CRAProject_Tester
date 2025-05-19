#include "tester.h"
void SSDTest::run(string command1, string command2)
{
	if (command1 == "1_" || command1 == "1_FullWriteAndReadCompare")
	{
		std::cout << "test1\n";
	}
	else if (command1 == "2_" || command1 == "2_PartialLBAWrite")
	{
		std::cout << "test2\n";
	}
	else if (command1 == "3_" || command1 == "3_WriteReadAging")
	{
		std::cout << "test3\n";
	}
	else
	{
		std::cout << "input error\n";
	}
}

