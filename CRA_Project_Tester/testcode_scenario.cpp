#include "gmock/gmock.h"
#include "testScript.h"
#include"Logger.h"
#include "testShell.h"
#include"Util.h"
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

class SSDTestFixturePartialLBAWrite : public ::testing::Test {
protected:
	MockWriteDriver_ mockWrite;
	MockReadDriver_ mockRead;
	SSDTest_PartialLBAWrite* ssd;

	void SetUp() override {
		ssd = new SSDTest_PartialLBAWrite(&mockWrite, &mockRead);
	}

	void TearDown() override {
		delete ssd;
	}
};

class SSDTestFixtureWriteReadAging : public ::testing::Test {
protected:
	MockWriteDriver_ mockWrite;
	MockReadDriver_ mockRead;
	SSDTest_WriteReadAging* ssd;

	void SetUp() override {
		ssd = new SSDTest_WriteReadAging(&mockWrite, &mockRead);
	}

	void TearDown() override {
		delete ssd;
	}
};

class SSDTestFixtureEraseAndWriteAging : public ::testing::Test {
protected:
	MockWriteDriver_ mockWrite;
	MockReadDriver_ mockRead;
	MockEraseDriver_ mockErase;
	SSDTest_EraseAndWriteAging* ssd;

	void SetUp() override {
		ssd = new SSDTest_EraseAndWriteAging(&mockWrite, &mockRead, &mockErase);
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

TEST_F(SSDTestFixturePartialLBAWrite, PartialLBAWrite)
{
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

TEST_F(SSDTestFixtureWriteReadAging, WriteReadAging)
{
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

TEST_F(SSDTestFixtureEraseAndWriteAging, EraseAndWriteAging)
{
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
	vector<string> filenames = { "until_.log","until_2.log" };
	for (auto& filename : filenames) {
		makeFile(filename);
	}

	LOG("ZIP TEST\n");

	for (auto& filename : filenames) {
		removeFile(filename);
	}
}

TEST_F(LoggerTest, 10KBTest) {

	const std::string filename = "latest.log";
	const size_t fileSizeInBytes = 11 * 1024; // 10KB = 10 * 1024 bytes

	std::ofstream file(filename, std::ios::binary);
	if (file.is_open()) {
		std::string content(fileSizeInBytes, 'A'); // 'A' ���ڷ� 10KB ä��
		file << content;
		file.close();
	}
	LOG("10KB TEST\n");
}

TEST_F(SSDTestFixturePartialLBAWrite, PartialLBAWrite_SSD)
{
	for (int i = 0; i < 30; i++)
	{
		EXPECT_CALL(mockWrite, run("4", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("0", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("3", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("1", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("2", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockRead, read("4"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("3"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("2"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("1"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("0"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
	}


	ssd->run("", "");
}

TEST_F(SSDTestFixturePartialLBAWrite, PartialLBAWrite_SSD_exception)
{
	for (int i = 0; i < 30; i++)
	{
		EXPECT_CALL(mockWrite, run("4", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("0", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("3", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("1", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockWrite, run("2", _))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address, std::string data) {
			mockWrite.writtenData[address] = data;
				});
		EXPECT_CALL(mockRead, read("4"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("3"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("2"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read("1"))
			.Times(::testing::AnyNumber())
			.WillRepeatedly([this](std::string address) {
			return mockWrite.writtenData[address];
				});
		EXPECT_CALL(mockRead, read(_))
			.Times(::testing::AnyNumber())
			.WillOnce(::testing::Return("0x12341234"));
	}


	EXPECT_THROW(ssd->run("", ""), exception);
}


TEST_F(SSDTestFixtureWriteReadAging, WriteReadAging_SSD)
{
	EXPECT_CALL(mockWrite, run("0", _))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address, std::string data) {
		mockWrite.writtenData[address] = data;
			});
	EXPECT_CALL(mockWrite, run("99", _))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address, std::string data) {
		mockWrite.writtenData[address] = data;
			});

	EXPECT_CALL(mockRead, read("0"))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address) {
		return mockWrite.writtenData[address];
			});

	EXPECT_CALL(mockRead, read("99"))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address) {
		return mockWrite.writtenData[address];
			});

	ssd->run("", "");
}

TEST_F(SSDTestFixtureWriteReadAging, WriteReadAging_SSD_exception)
{
	EXPECT_CALL(mockWrite, run("0", _))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address, std::string data) {
		mockWrite.writtenData[address] = data;
			});
	EXPECT_CALL(mockWrite, run("99", _))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address, std::string data) {
		mockWrite.writtenData[address] = data;
			});

	EXPECT_CALL(mockRead, read("0"))
		.Times(::testing::AnyNumber())
		.WillRepeatedly([this](std::string address) {
		return mockWrite.writtenData[address];
			});
	EXPECT_CALL(mockRead, read("99"))
		.Times(::testing::AnyNumber())
		.WillOnce(::testing::Return("0x12341234"));

	EXPECT_THROW(ssd->run("", ""), exception);

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