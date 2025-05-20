#include "testShell.h"
#include "Util.h"
#include "testScript.h"
#include <stdexcept>

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
	//string buffer = createRadomString(); //read 구현 전까지 buffer에 임의값 고정하여 TEST 돌림
	string buffer = "0x11111111";

	for (int i = 0; i < 100; i += 5)
	{
		for (int j = i; j < i + 5; j++) {
			write->run(to_string(j), buffer);
			writeBuffer[j] = buffer;
		}
		for (int j = i; j < i + 5; j++) {
			readBuffer = read->read(to_string(j));
			if (CompareData(writeBuffer[j], readBuffer)) continue;
		}
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

void SSDTest_EraseAndWriteAging::run(string param1, string param2)
{
	if (mWrite == nullptr)    mWrite = new Write;
	if (mRead == nullptr)    mRead = new Read;
	if (mErase == nullptr)    mErase = new Erase;

	mErase->run("0");
	mErase->run("1");
	mErase->run("2");

	for (int loop = 0; loop < 30; loop++) {
		WriteAndErase(2);
		WriteAndErase(4);
		WriteAndErase(6);
	}
}

void SSDTest_EraseAndWriteAging::WriteAndErase(int start_addr)
{
	string wdata = createRadomString();
	string rdata, ov_rdata;

	// s1. write
	mWrite->run(to_string(start_addr), wdata);
	rdata = mRead->read(to_string(start_addr));

	mWrite->run(to_string(start_addr), wdata);
	ov_rdata = mRead->read(to_string(start_addr));

	// s2. ReadCompare 
	if (rdata != ov_rdata) throw std::exception();

	// s3. erase
	mErase->run(to_string(start_addr));
	mErase->run(to_string(start_addr + 1));
	mErase->run(to_string(start_addr + 2));
}