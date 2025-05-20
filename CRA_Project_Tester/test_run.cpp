#include "gmock/gmock.h"
#include <string>
#include "testScript.h"
#include"Logger.h"
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

class SSDTestFixture : public ::testing::Test {
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

TEST_F(SSDTestFixture, FullWriteAndReadCompareFailTest) {
    EXPECT_CALL(mockWrite, run(_, _)).Times(::testing::AnyNumber());
    EXPECT_CALL(mockRead, read(_))
        .Times(::testing::AnyNumber())
        .WillOnce(::testing::Return("0x12341234"));

    EXPECT_THROW(ssd->run("", ""), exception);
}

TEST_F(SSDTestFixture, FullWriteAndReadCompareTest) {

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

class LoggerTest : public ::testing::Test {
protected:
    std::string logFilename = "test_log.txt";

    void SetUp() override {
        std::remove(logFilename.c_str());
    }

    void TearDown() override {
        std::remove(logFilename.c_str());
    }
};

TEST_F(LoggerTest, LogMessageIsWrittenToFile) {
    Logger logger(logFilename);

    std::string testMessage = "This is a test log entry.";
    logger.print(1,"LoggerTestFunc", testMessage);

    std::ifstream infile(logFilename);
    ASSERT_TRUE(infile.is_open()) << "Log file could not be opened.";

    std::string line;
    bool found = false;

    while (std::getline(infile, line)) {
        if (line.find(testMessage) != std::string::npos &&
            line.find("LoggerTestFunc( )") != std::string::npos) {
            found = true;
            break;
        }
    }

    infile.close();
    ASSERT_TRUE(found) << "Log message not found in log file.";
}