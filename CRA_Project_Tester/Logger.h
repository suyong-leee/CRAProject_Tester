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
	void print(std::string function , std::string msg);
	void setLogType(int type);
	std::string getNewfileName( tm& localTime);
	std::ostringstream getprintFormat(tm& localTime, std::string& function, std::string& msg);
private:
	std::string logFileName ; 
	int Default = 0;
};

#define LOG(msg) loggerInstance.print(__FUNCTION__, msg)
extern Logger loggerInstance;
