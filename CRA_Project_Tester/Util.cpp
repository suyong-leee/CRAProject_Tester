#include "Util.h"

string createRandomString(void) {
	stringstream ss;
	unsigned int randomNum = rand();

	randomNum = randomNum << 16 | rand();
	ss << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << randomNum;
	return "0x" + ss.str();
}

bool CompareData(string  writeData, string readData)
{
	if (writeData == readData) {
		return true;
	}
	else {
		std::cout << "FAIL\n";
		throw exception("Compare Failed\n");
	}
}
