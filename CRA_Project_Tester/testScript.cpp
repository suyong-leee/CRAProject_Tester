#include "testShell.h"
#include "Util.h"
#include "Logger.h"
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
	
	string readBuffer;
	string buffer; //read 구현 전까지 buffer에 임의값 고정하여 TEST 돌림

	if (mWrite == nullptr)    mWrite = new Write;
	if (mRead == nullptr)    mRead = new Read;

	for (int i = 0; i < 100; i += 5)
	{
		buffer = createRandomString(); //read 구현 전까지 buffer에 임의값 고정하여 TEST 돌림
		for (int j = i; j < i + 5; j++) {
			mWrite->run(to_string(j), buffer);
		}
		for (int j = i; j < i + 5; j++) {
			readBuffer = mRead->read(to_string(j));
			if (CompareData(buffer, readBuffer)) continue;
		}
	}

}

void SSDTest_PartialLBAWrite::run(string param1, string param2)
{
    string wdata = createRandomString();
    string rdata[5];

    if (mWrite == nullptr)    mWrite = new Write;
    if (mRead == nullptr)    mRead = new Read;

    for (int loop = 0; loop < 30; loop++)
	{
        // s1. write
        mWrite->run("4", wdata);
        mWrite->run("0", wdata);
        mWrite->run("3", wdata);
        mWrite->run("1", wdata);
        mWrite->run("2", wdata);

        // s2. ReadCompare
		CompareData(wdata, mRead->read("0"));
		CompareData(wdata, mRead->read("1"));
		CompareData(wdata, mRead->read("2"));
		CompareData(wdata, mRead->read("3"));
		CompareData(wdata, mRead->read("4"));
    }
}

void SSDTest_WriteReadAging::run(string param1, string param2)
{
	if (mWrite == nullptr)    mWrite = new Write;
	if (mRead == nullptr)    mRead = new Read;

	for (int loop = 0; loop < 200; loop++) {
		// s1. write
		string wdata1 = createRandomString();
		mWrite->run("0", wdata1);
		string wdata2 = createRandomString();
		mWrite->run("99", wdata2);

		// s2. ReadCompare 
		CompareData(wdata1, mRead->read("0"));
		CompareData(wdata2, mRead->read("99"));
	}
}

void SSDTest_FullScenario::run(string file_name, string param2)
{
	FILE* f;
	fopen_s(&f, file_name.c_str(), "r");
	if (!f) throw std::exception();

	Logger::getInstance().setLogType(1);

	char word[128];
	while (fgets(word, sizeof(word), f) != nullptr)
	{
		string tsname(word);
		ITestOperation* scenario = nullptr;

		if (tsname.find("1_") != string::npos) scenario = new SSDTest_FullWriteAndReadCompare(new Write, new Read);
		else if (tsname.find("2_") != string::npos) scenario = new SSDTest_PartialLBAWrite(new Write, new Read);
		else if (tsname.find("3_") != string::npos) scenario = new SSDTest_WriteReadAging(new Write, new Read);
		else if (tsname.find("4_") != string::npos) scenario = new SSDTest_EraseAndWriteAging(new Write, new Read, new Erase);
		else
		{
			Logger::getInstance().setLogType(Logger::NORMAL);
			fclose(f);
			throw std::exception();
		}

		try
		{
			if (!tsname.empty() && tsname.back() == '\n')    tsname.pop_back();
			LOG(tsname+"   ___   Run... ");
			Logger::getInstance().setLogType(Logger::RUNNER_EXCEPT);
			scenario->run("", "");
			Logger::getInstance().setLogType(Logger::RUNNER);
			LOG("Pass\n");

			delete scenario;
		}
		catch (exception& e)
		{
			Logger::getInstance().setLogType(Logger::RUNNER);
			LOG("FAIL!\n");

			Logger::getInstance().setLogType(Logger::NORMAL);
			fclose(f);
			throw std::exception();
		}
	}

	Logger::getInstance().setLogType(Logger::NORMAL);
	fclose(f);
}

void SSDTest_EraseAndWriteAging::run(string param1, string param2)
{
	if (mWrite == nullptr)    mWrite = new Write;
	if (mRead == nullptr)    mRead = new Read;
	if (mErase == nullptr)    mErase = new Erase;

	mErase->run("0", "3");

	for (int loop = 0; loop < 30; loop++)
		for(int addr = 2; addr <= 98 ; addr += 2)
		    WriteAndErase(addr);
  
}

void SSDTest_EraseAndWriteAging::WriteAndErase(int start_addr)
{
	string wdata;
	string rdata;

	// s1. write & ReadCompare
	wdata = createRandomString();
	mWrite->run(to_string(start_addr), wdata);
	rdata = mRead->read(to_string(start_addr));
	CompareData(wdata, rdata);

	// s2. rewrite & ReadCompare
	wdata = createRandomString();
	mWrite->run(to_string(start_addr), wdata);
	rdata = mRead->read(to_string(start_addr));
	CompareData(wdata, rdata);	

	// s3. erase
	mErase->run(to_string(start_addr), "3");
}
