#include "gmock/gmock.h"
#include "tester.h"
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