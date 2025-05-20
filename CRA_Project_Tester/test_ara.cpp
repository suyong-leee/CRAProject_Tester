#include "gmock/gmock.h"
#include "testShell.h"
#include "testScript.h"
#include <string>

using namespace testing;

class MockRead : public Read {
public:
	MOCK_METHOD(void, run, (string, string), (override));
	MOCK_METHOD(string, read, (string), ());
};

class MockWrite : public Write {
public:
	MOCK_METHOD(void, run, (string, string), (override));
};

class MockErase : public Erase {
public:
	MOCK_METHOD(void, run, (string, string), (override));
};

TEST(SDDTEST, PartialLBAWrite)
{
	MockWrite mkwr;
	MockRead mkrd;
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

TEST(SDDTEST, PartialLBAWrite_exception)
{
	MockWrite mkwr;
	MockRead mkrd;
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

TEST(SDDTEST, WriteReadAging)
{
	MockWrite mkwr;
	MockRead mkrd;
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

TEST(SDDTEST, WriteReadAging_exception)
{
	MockWrite mkwr;
	MockRead mkrd;
	SSDTest_WriteReadAging test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("0", _));
	EXPECT_CALL(mkwr, run("99", _));

	EXPECT_CALL(mkrd, read("0"))
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("99"))
		.WillRepeatedly(Return(string("0x11111111")));

	EXPECT_THROW(test.run("", ""), exception);
}

TEST(SDDTEST, EraseAndWriteAging)
{
	MockWrite mkwr;
	MockRead mkrd;
	MockErase mker;
	SSDTest_EraseAndWriteAging test(&mkwr, &mkrd, &mker);

	EXPECT_CALL(mkwr, run("2", "0x23456781"));
	EXPECT_CALL(mkwr, run("2", "0x22222222"));
	EXPECT_CALL(mker, run("2", ""));
	EXPECT_CALL(mker, run("3", ""));
	EXPECT_CALL(mker, run("4", ""));

	EXPECT_CALL(mkwr, run("4", "0x456789012"));
	EXPECT_CALL(mkwr, run("4", "0x444444444"));
	EXPECT_CALL(mker, run("4", ""));
	EXPECT_CALL(mker, run("5", ""));
	EXPECT_CALL(mker, run("6", ""));

	EXPECT_CALL(mkwr, run("6", "0x67890123"));
	EXPECT_CALL(mkwr, run("6", "0x66666666"));
	EXPECT_CALL(mker, run("6", ""));
	EXPECT_CALL(mker, run("7", ""));
	EXPECT_CALL(mker, run("8", ""));

	test.run("", "");
}