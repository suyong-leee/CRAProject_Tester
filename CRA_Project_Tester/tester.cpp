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
    string wdata = "0x12345678";
    string rdata[5];

    if (mWrite == nullptr)    mWrite = new Write;
    if (mRead == nullptr)    mRead = new Read;

    for (int loop = 0; loop < 30; loop++) {
        // s1. write
        mWrite->run("4", wdata);
        mWrite->run("0", wdata);
        mWrite->run("3", wdata);
        mWrite->run("1", wdata);
        mWrite->run("2", wdata);

        // s2. ReadCompare
        rdata[0] = mRead->read("0");
        rdata[1] = mRead->read("1");
        rdata[2] = mRead->read("2");
        rdata[3] = mRead->read("3");
        rdata[4] = mRead->read("4");

        for (int i = 0; i < 4; i++) {
            if (rdata[i] != rdata[i + 1]) {
                throw std::exception();
            }
        }
    }
}

void SSDTest_WriteReadAging::run(string param1, string param2)
{
	string wdata = createRadomString();

	if (mWrite == nullptr)    mWrite = new Write;
	if (mRead == nullptr)    mRead = new Read;

	for (int loop = 0; loop < 200; loop++) {
		// s1. write
		mWrite->run("0", wdata);
		mWrite->run("99", wdata);

		// s2. ReadCompare 
		if (mRead->read("0") != mRead->read("99")) throw std::exception();
	}
}

string SSDTest_WriteReadAging::createRadomString(void) {
	return "0x12345678"
}