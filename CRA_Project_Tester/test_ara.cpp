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

TEST(SDDTEST, DISABLED_PartialLBAWrite)
{
	/*
	* To perform this test,
	* modify the createRandomString function in Util.cpp as below and perform this test.
	* string createRandomString(void)
	* {
	*     return "0x12345678"
	* }
	*/
	MockWrite mkwr;
	MockRead mkrd;
	SSDTest_PartialLBAWrite test(&mkwr, &mkrd);

	EXPECT_CALL(mkwr, run("4", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("0", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("3", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("2", "0x12345678")).Times(30);
	EXPECT_CALL(mkwr, run("1", "0x12345678")).Times(30);

	EXPECT_CALL(mkrd, read("0"))
		.Times(30)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("1"))
		.Times(30)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("2"))
		.Times(30)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("3"))
		.Times(30)
		.WillRepeatedly(Return(string("0x12345678")));
	EXPECT_CALL(mkrd, read("4"))
		.Times(30)
		.WillRepeatedly(Return(string("0x12345678")));

	test.run("", "");
}

TEST(SDDTEST, DISABLED_PartialLBAWrite_exception)
{
	/*
	* To perform this test,
	* modify the createRandomString function in Util.cpp as below and perform this test.
	* string createRandomString(void)
	* {
	*     return "0x12345678"
	* }
	*/
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

TEST(SDDTEST, DISABLED_WriteReadAging)
{
	/*
	* To perform this test,
	* modify the createRandomString function in Util.cpp as below and perform this test.
	* string createRandomString(void)
	* {
	*     return "0x12345678"
	* }
	*/
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

TEST(SDDTEST, DISABLED_WriteReadAging_exception)
{
	/*
	* To perform this test,
	* modify the createRandomString function in Util.cpp as below and perform this test.
	* string createRandomString(void)
	* {
	*     return "0x12345678"
	* }
	*/
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

TEST(SDDTEST, DISABLED_EraseAndWriteAging)
{
	/*
	* 해당 테스트를 수행하려면
	* Util.cpp에서 아래와 같이 createRandomString 함수를 수정한 후 검증수행
	* string createRandomString(void)
	* {
	*     return "0x12345678"
	* }
	*/
	MockWrite mkwr;
	MockRead mkrd;
	MockErase mker;
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