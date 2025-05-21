#include "gmock/gmock.h"
#include "testScript.h"
#include"Logger.h"
#include "testShell.h"
#include <string>
using namespace std;
using namespace testing;


class MockWriteDriver_ : public Write {
public:
	MOCK_METHOD(void, run, (std::string, std::string), (override));
	MOCK_METHOD(void, write, (std::string address, std::string data), ());
	std::map<std::string, std::string> writtenData;

	void writeData(const std::string& address, const std::string& data) {
		writtenData[address] = data;
	}
};

class MockReadDriver_ : public Read {
public:
	MOCK_METHOD(void, run, (std::string, std::string), (override));
	MOCK_METHOD(string, read, (std::string address), ());
	MockWriteDriver_* mockWriteDriver;

	std::string readData(const std::string& address) {
		auto it = mockWriteDriver->writtenData.find(address);
		if (it != mockWriteDriver->writtenData.end()) {
			return it->second;
		}
		return "";
	}
};

class MockEraseDriver_ : public Erase {
public:
	MOCK_METHOD(void, run, (string, string), (override));
};

class SSDTestFixtureFullWriteAndReadCompare : public ::testing::Test {
protected:
	MockWriteDriver_ mockWrite;
	MockReadDriver_ mockRead;
	SSDTest_FullWriteAndReadCompare* ssd;

	void SetUp() override {
		ssd = new SSDTest_FullWriteAndReadCompare(&mockWrite, &mockRead);
	}

	void TearDown() override {
		delete ssd;
	}
};

class LoggerTest : public ::testing::Test {
protected:
	std::string logFilename = "latest.log";

	void SetUp() override {
		std::remove(logFilename.c_str());
	}

	void TearDown() override {
		std::remove(logFilename.c_str());
	}
};

TEST_F(SSDTestFixtureFullWriteAndReadCompare, FullWriteAndReadCompareFailTest) {
	EXPECT_CALL(mockWrite, run(_, _)).Times(::testing::AnyNumber());
	EXPECT_CALL(mockRead, read(_))
		.Times(::testing::AnyNumber())
		.WillOnce(::testing::Return("0x12341234"));

	EXPECT_THROW(ssd->run("", ""), exception);
}

TEST_F(SSDTestFixtureFullWriteAndReadCompare, FullWriteAndReadCompareTest) {

	EXPECT_CALL(mockWrite, run(_, _))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address, std::string data) {
		mockWrite.writtenData[address] = data;
			});


	EXPECT_CALL(mockRead, read(_))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address) {
		return mockWrite.writtenData[address];
			});

	ssd->run("", "");
}

TEST_F(LoggerTest, LogMessageIsWrittenToFile) {

	std::string testMessage = "This is a test log entry";
	LOG(testMessage);
	std::ifstream infile(logFilename);
	ASSERT_TRUE(infile.is_open()) << "Log file could not be opened.";

	std::string line;
	bool found = false;

	while (std::getline(infile, line)) {
		if (line.find(testMessage) != std::string::npos)
		{
			found = true;
			break;
		}
	}

	infile.close();
	ASSERT_TRUE(found) << "Log message not found in log file.";
}

TEST_F(LoggerTest, makeZipTest) {
	LOG("ZIP TEST START\n");
	std::string filename = "until_.log";
	std::ofstream logFile(filename);
	if (logFile.is_open()) {
		logFile << "This is a log entry." << std::endl;
		logFile.close();
		std::cout << "Log written to " << filename << "\n";
	}


	filename = "until_2.log";
	std::ofstream logFile2(filename);
	if (logFile2.is_open()) {
		logFile2 << "This is a log entry2." << std::endl;
		logFile2.close();
		std::cout << "Log written to " << filename << "\n";
	}

	LOG("ZIP TEST\n");

	if (std::remove(filename.c_str()) == 0) {
		std::cout << filename << " deleted successfully.\n";
	}

	filename = "until_.log";
	if (std::remove(filename.c_str()) == 0) {
		std::cout << filename << " deleted successfully.\n";
	}

}

TEST_F(LoggerTest, 10KBTest) {

	const std::string filename = "latest.log";
	const size_t fileSizeInBytes = 11 * 1024; // 10KB = 10 * 1024 bytes

	std::ofstream file(filename, std::ios::binary);
	if (file.is_open()) {
		std::string content(fileSizeInBytes, 'A'); // 'A' 문자로 10KB 채움
		file << content;
		file.close();
	}
	LOG("10KB TEST\n");
}

TEST(SDDTEST, DISABLED_PartialLBAWrite)
{
	MockWriteDriver_ mkwr;
	MockReadDriver_ mkrd;
	SSDTest_PartialLBAWrite test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("4", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("0", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("3", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("2", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("1", "0x12345678")).Times(30);

	EXPECT_CALL(mkrd, read("0")).Times(30);
	EXPECT_CALL(mkrd, read("1")).Times(30);
	EXPECT_CALL(mkrd, read("2")).Times(30);
	EXPECT_CALL(mkrd, read("3")).Times(30);
	EXPECT_CALL(mkrd, read("4")).Times(30);

	test.run("", "");
}

TEST(SDDTEST, DISABLED_PartialLBAWrite_exception)
{
	MockWriteDriver_ mkwr;
	MockReadDriver_ mkrd;
	SSDTest_PartialLBAWrite test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("4", "0x12345678"));
	EXPECT_CALL(mkwr, run("0", "0x12345678"));
	EXPECT_CALL(mkwr, run("3", "0x12345678"));
	EXPECT_CALL(mkwr, run("2", "0x12345678"));
	EXPECT_CALL(mkwr, run("1", "0x12345678"));

	EXPECT_CALL(mkrd, read("0"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("1"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("2"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("3"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("4"))
		.WillRepeatedly(Return(string("0x11111111")));

	EXPECT_THROW(test.run("", ""), exception);
}

TEST(SDDTEST, DISABLED_WriteReadAging)
{
	MockWriteDriver_ mkwr;
	MockReadDriver_ mkrd;
	SSDTest_WriteReadAging test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("0", _)).Times(200);
	EXPECT_CALL(mkwr, run("99", _)).Times(200);

	EXPECT_CALL(mkrd, read("0"))
		.Times(200)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("99"))
		.Times(200)
		.WillRepeatedly(Return(string("0x12345678")));

	test.run("", "");
}

TEST(SDDTEST, DISABLED_WriteReadAging_exception)
{
	MockWriteDriver_ mkwr;
	MockReadDriver_ mkrd;
	SSDTest_WriteReadAging test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("0", _));
	EXPECT_CALL(mkwr, run("99", _));

	EXPECT_CALL(mkrd, read("0"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("99"))
		.WillRepeatedly(Return(string("0x11111111")));

	EXPECT_THROW(test.run("", ""), exception);
}

TEST(SDDTEST, DISABLED_EraseAndWriteAging)
{
	MockWriteDriver_ mkwr;
	MockReadDriver_ mkrd;
	MockEraseDriver_ mker;
	SSDTest_EraseAndWriteAging test(&mkwr, &mkrd, &mker);

	// erase 0 ~2
	EXPECT_CALL(mker, run("0", ""));
	EXPECT_CALL(mker, run("1", ""));
	//	EXPECT_CALL(mker, run("2", ""));

		// write 2 & erase 2~4
	EXPECT_CALL(mkwr, run("2", _))
		.Times(60);
	EXPECT_CALL(mkrd, read("2"))
		.Times(60)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mker, run("2", ""))
		.Times(31);
	EXPECT_CALL(mker, run("3", ""))
		.Times(30);
	//	EXPECT_CALL(mker, run("4", ""))
	//		.Times(30);

		// write 4 & erase 4~6
	EXPECT_CALL(mkwr, run("4", _))
		.Times(60);
	EXPECT_CALL(mkrd, read("4"))
		.Times(60)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mker, run("4", ""))
		.Times(60);
	EXPECT_CALL(mker, run("5", ""))
		.Times(30);
	//	EXPECT_CALL(mker, run("6", ""))
	//		.Times(30);

		// write 6 & erase 6~8
	EXPECT_CALL(mkwr, run("6", _))
		.Times(60);
	EXPECT_CALL(mkrd, read("6"))
		.Times(60)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mker, run("6", ""))
		.Times(60);
	EXPECT_CALL(mker, run("7", ""))
		.Times(30);
	EXPECT_CALL(mker, run("8", ""))
		.Times(30);

	test.run("", "");
}