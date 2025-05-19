#include "tester.h"
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

void SSDTest_FullWriteAndReadCompare::run(string command1, string command2)
{
	//• 0 ~4번 LBA까지 Write 명령어를 수행한다.
	//• 0 ~4번 LBA까지 ReadCompare 수행
	//• 5 ~9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	//• 5 ~9번 LBA까지 ReadCompare 수행
	//• 10 ~14번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	//• 10 ~14번 LBA까지 ReadCompare 수행
	//• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare를 수행한다.
	string writeBuffer[100];
	string readBuffer;
	string buffer = "0x11111111";
	for (int i = 0; i < 100; i += 5)
	{
		for (int j = i; j < i + 5; j++) {
			saveBuffer(j, buffer, writeBuffer[j]);
		}
		for (int j = i; j < i + 5; j++) {
			readBuffer = read->read(to_string(j));
			if (CompareBuffer(writeBuffer[j], readBuffer)) continue;
		}
	}

}

void SSDTest_FullWriteAndReadCompare::saveBuffer(int j, std::string buffer, std::string&  writeBuffer)
{
	write->run(to_string(j), buffer);
	writeBuffer = buffer;
}

bool SSDTest_FullWriteAndReadCompare::CompareBuffer(std::string  writeBuffer,  std::string readBuffer)
{
	if (writeBuffer == readBuffer) {
		return true;
	}
	else {
		throw exception("Compare Failed\n");
	}
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
	stringstream ss;
	unsigned int randomNum = rand();

	randomNum = randomNum << 16 | rand();
	ss << hex << randomNum;
	return "0x" + ss.str();
}