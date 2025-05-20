#include "Util.h"

string createRadomString(void) {
	stringstream ss;
	unsigned int randomNum = rand();

	randomNum = randomNum << 16 | rand();
	ss << hex << randomNum;
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
