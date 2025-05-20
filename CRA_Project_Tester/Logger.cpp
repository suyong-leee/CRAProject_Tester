#include"Logger.h"
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
            std::time_t now = std::time(nullptr);
            std::tm localTime;
            localtime_s(&localTime, &now);

            std::ostringstream newFileName;
            newFileName << "until_"
                << std::setw(2) << std::setfill('0') << (localTime.tm_year % 100) << "."
                << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1) << "."
                << std::setw(2) << std::setfill('0') << localTime.tm_mday << "_"
                << std::setw(2) << std::setfill('0') << localTime.tm_hour << "."
                << std::setw(2) << std::setfill('0') << localTime.tm_min << "."
                << std::setw(2) << std::setfill('0') << localTime.tm_sec << ".log";

            std::string newLogFileName = newFileName.str();

            std::rename(logFileName.c_str(), newLogFileName.c_str());
        }
    }


    std::ofstream logFile(logFileName, std::ios::app);
    if (logFile.is_open()) {
        logFile << oss.str() << std::endl;
    }

}
