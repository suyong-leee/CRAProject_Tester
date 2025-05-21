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
	static Logger& getInstance() {
		static Logger instance("latest.log");
		return instance;
	}

	void print(std::string function , std::string msg);
	bool IsScript();
	void findUntilFile(std::vector<std::filesystem::directory_entry>& logFiles);
	void checkFileSize(std::ifstream& file, tm& localTime, std::error_code& ec);
	void setLogType(int type);
	std::string getNewfileName( tm& localTime);
	std::ostringstream getprintFormat(tm& localTime, std::string& function, std::string& msg);
	void makeZip(std::vector<std::filesystem::directory_entry>& logFiles, std::error_code& ec);
private:
	Logger(const std::string& filename);  // private 생성자
	Logger(const Logger&) = delete;       // 복사 방지
	Logger& operator=(const Logger&) = delete;
	std::string logFileName ; 
	int Default = 0;
};

#define LOG(msg) Logger::getInstance().print(__FUNCTION__, msg)

