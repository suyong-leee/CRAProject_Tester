#include "tester.h"
void SSDTest_FullWriteAndReadCompare::run(string command1, string command2)
{
	write->run("", "");
	write->run("", "");
	write->run("", "");
	write->run("", "");

	read->run("");
	read->run("");
	read->run("");
	read->run("");

}

void SSDTest_PartialLBAWrite::run(string param1, string param2)
{
}

void SSDTest_WriteReadAging::run(string param1, string param2)
{
}
