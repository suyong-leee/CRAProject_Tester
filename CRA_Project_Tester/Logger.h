#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

class Logger {
public:
	Logger(const std::string& filename = "latest.log") : logFileName(filename) {}
	void print(int type, std::string function , std::string msg);
private:
	std::string logFileName ; 
};

#define LOG(msg) loggerInstance.print(0,__FUNCTION__, msg)
extern Logger loggerInstance;