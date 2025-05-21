#include"Logger.h"
#include "Util.h"
using namespace filesystem;
using namespace std;
Logger::Logger(const string& filename) : logFileName(filename) {
}

void Logger::print(string function, string msg)
{
	time_t now = time(nullptr);
	tm localTime;
	error_code ec;
	vector<directory_entry> logFiles;
	localtime_s(&localTime, &now);

	ostringstream oss = getprintFormat(localTime, function, msg);

	ifstream file(logFileName, ios::ate | ios::binary);

	if (file.is_open()) 
	{

		checkFileSize(file, localTime, ec);

		findUntilFile(logFiles);

		if (logFiles.size() >= 2)
		{
			makeZip(logFiles, ec);
		}
	}

	ofstream logFile(logFileName,ios::app);
	if (logFile.is_open()) 
	{
		logFile << oss.str();
	}
	if (Default != RUNNER_EXCEPT) 
	{
		std::cout << oss.str();
	}
}

void Logger::makeZip(vector<directory_entry> &logFiles, error_code& ec) {
	sort(logFiles.begin(), logFiles.end(), [](const auto& a, const auto& b) {
		return last_write_time(a) < last_write_time(b);
		});

	const auto& oldestFile = logFiles.front();
	path newPath = oldestFile.path();
	newPath.replace_extension(".zip");

	rename(oldestFile.path(), newPath, ec);
	if (ec) {
		cerr << "Failed to rename " << oldestFile.path() << " to " << newPath << ": " << ec.message() << '\n';
	}
}

void Logger::findUntilFile(vector<filesystem::directory_entry>& logFiles)
{
	for (const auto& entry : directory_iterator(".")) {
		if (entry.is_regular_file()) {
			string filename = entry.path().filename().string();
			if (filename.rfind("until_", 0) == 0 && ends_with(filename, ".log")) {
				logFiles.push_back(entry);
			}
		}
	}
}

void Logger::checkFileSize(ifstream& file, tm& localTime, error_code& ec)
{
	streampos size = file.tellg();
	file.close();

	if (size > 10 * 1024) 
	{
		string newLogFileName = getNewfileName(localTime);
		rename(logFileName.c_str(), newLogFileName.c_str(), ec);
		if (ec) 
		{
			cerr << "Failed to rename " << logFileName.c_str() << " to " << newLogFileName.c_str() << ": " << ec.message() << '\n';
		}
	}
}

void Logger::setLogType(int type) 
{
	Default = type;
}

string Logger::getNewfileName(tm& localTime)
{
	ostringstream newFileName;
	newFileName << "until_"
		<< setw(2) << setfill('0') << (localTime.tm_year % 100)
		<< setw(2) << setfill('0') << (localTime.tm_mon + 1)
		<< setw(2) << setfill('0') << localTime.tm_mday << "_"
		<< setw(2) << setfill('0') << localTime.tm_hour << "h_"
		<< setw(2) << setfill('0') << localTime.tm_min << "m_"
		<< setw(2) << setfill('0') << localTime.tm_sec << "s.log";
	return newFileName.str();
}

ostringstream Logger::getprintFormat(tm& localTime, string& function, string& msg)
{
	ostringstream oss;
	if (Default == NORMAL)
	{
		oss << "[" << setw(2) << setfill('0') << (localTime.tm_year % 100) << "."
			<< setw(2) << setfill('0') << (localTime.tm_mon + 1) << "."
			<< setw(2) << setfill('0') << localTime.tm_mday << " "
			<< setw(2) << setfill('0') << localTime.tm_hour << ":"
			<< setw(2) << setfill('0') << localTime.tm_min << "] ";

		string fullFunc = function + "( )";
		oss << left << setw(40) << setfill(' ') << fullFunc;
		oss << ": ";
	}
	oss << msg;
	return oss;
}
