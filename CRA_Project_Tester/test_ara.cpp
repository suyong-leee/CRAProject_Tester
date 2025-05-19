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


TEST(SDDTEST, Test3)
{
	MockWrite mkwr;
	MockRead mkrd;
	SSDTest_PartialLBAWrite ssdtest3(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("4", "0x12345678"));
	EXPECT_CALL(mkwr, run("0", "0x12345678"));
	EXPECT_CALL(mkwr, run("3", "0x12345678"));
	EXPECT_CALL(mkwr, run("2", "0x12345678"));
	EXPECT_CALL(mkwr, run("1", "0x12345678"));

	ssdtest3.run("","");
}

TEST(SDDTEST, WriteReadAging)
{
	MockWrite mkwr;
	MockRead mkrd;
	SSDTest_WriteReadAging test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("0", "0x12345678")).Times(200);
	EXPECT_CALL(mkwr, run("99", "0x12345678")).Times(200);

	EXPECT_CALL(mkrd, read("0"))
		.Times(200)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("99"))
		.Times(200)
		.WillRepeatedly(Return(string("0x12345678")));

	test.run("", "");
}