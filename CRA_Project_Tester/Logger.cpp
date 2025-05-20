#include"Logger.h"
#include "Util.h"
void Logger::print(int type, std::string function, std::string msg)
{
    std::time_t now = std::time(nullptr);
    std::tm localTime;

    localtime_s(&localTime, &now);

    std::ostringstream oss;

    // [YY.MM.DD HH:MM]
    oss << "[" << std::setw(2) << std::setfill('0') << (localTime.tm_year % 100) << "."
        << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1) << "."
        << std::setw(2) << std::setfill('0') << localTime.tm_mday << " "
        << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
        << std::setw(2) << std::setfill(' ') << localTime.tm_min << "] ";

    std::string fullFunc = function + "( )";
    oss << std::left << std::setw(40) << fullFunc; 
    oss << ": " << msg;

    std::ifstream file(logFileName, std::ios::ate | std::ios::binary);
    if (file.is_open()) {

        std::streampos size = file.tellg();
        file.close();


        if (size > 10 * 1024) { 
            std::ostringstream newFileName;
            newFileName << "until_"
                << std::setw(2) << std::setfill('0') << (localTime.tm_year % 100)
                << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1)
                << std::setw(2) << std::setfill('0') << localTime.tm_mday << "_"
                << std::setw(2) << std::setfill('0') << localTime.tm_hour << "h_"
                << std::setw(2) << std::setfill('0') << localTime.tm_min << "m_"
                << std::setw(2) << std::setfill('0') << localTime.tm_sec << "s.log";

            std::string newLogFileName = newFileName.str();

            std::rename(logFileName.c_str(), newLogFileName.c_str());
        }

        namespace fs = std::filesystem;
        std::vector<fs::directory_entry> logFiles;

        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.rfind("until_", 0) == 0 && ends_with(filename, ".log")) {
                    logFiles.push_back(entry);
                }
            }
        }

        if (logFiles.size() > 2)
        {
            // sort oldest file
            std::sort(logFiles.begin(), logFiles.end(), [](const auto& a, const auto& b) {
                return fs::last_write_time(a) < fs::last_write_time(b);
                });

            const auto& oldestFile = logFiles.front();
            fs::path newPath = oldestFile.path();
            newPath.replace_extension(".zip");

            std::error_code ec;
            fs::rename(oldestFile.path(), newPath, ec);
            if (ec) {
                std::cerr << "Failed to rename " << oldestFile.path() << " to " << newPath << ": " << ec.message() << '\n';
            }
        }
    }

    std::ofstream logFile(logFileName, std::ios::app);
    if (logFile.is_open()) {
        logFile << oss.str() << std::endl;
    }

}
