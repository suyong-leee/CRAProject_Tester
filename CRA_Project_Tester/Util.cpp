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
		throw exception("Compare Failed\n");
	}
}

bool ends_with(const std::string& str, const std::string& suffix) {
	if (str.length() >= suffix.length()) {
		return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
	}
	else {
		return false;
	}
}
