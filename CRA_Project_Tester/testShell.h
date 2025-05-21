#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "Logger.h"

constexpr int MAX_LBA = 99;
constexpr int ERASE_BLOCK_SIZE = 10;
constexpr int FULL_READ_SIZE = 100;

using namespace std;

class Validator {
public:
	static bool checkLBA(const string& lba) {
		if (lba.empty() || lba.size() > 2)
			throw invalid_argument("LBA 범위는 0 ~ 99");

		for (char ch : lba) {
			if (!isdigit(ch))
				throw invalid_argument("0~9 사이 수만 가능");
		}
		return true;
	}

	static bool checkHexData(const string& data) {
		if (data.size() != 10)
		{
			throw invalid_argument("16진수 8자리만 입력가능");
		}
		for (int i = 2; i < data.size(); i++)
		{
			if ((data[i] >= '0' && data[i] <= '9') || (data[i] >= 'A' && data[i] <= 'F')) continue;
			else throw invalid_argument("0 ~ 9사이 A~F만 가능");
		}
		return true;
	}
};

class SSDCommandExecutor {
public:
	static bool runCommand(const string& command) {
		FILE* pipe = _popen(command.c_str(), "r");
		if (!pipe) {
			LOG("[ERROR] Failed to execute: " + command);
			return false;
		}
		_pclose(pipe);
		return true;
	}

	static string runReadOutput(const string& outputFile = "ssd_output.txt") {

		string result;
		ifstream file(outputFile);
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				result += line;
			}
			file.close();
		}
		else {
			result = "error: cannot open output file";
		}
		return result;
	}
};

class ITestOperation
{
public:
	virtual void run(string command = "", string command2 = "") = 0;

};
class Flush : public ITestOperation
{
public:

	void run(string command1 = "", string command2 = "") override
	{
		string command = "ssd.exe F";
		flushSSD(command);
	}

	void flushSSD(string command)
	{
		SSDCommandExecutor::runCommand(command);
	}
	
};



class Erase : public ITestOperation
{
public:

	void run(string command1 = "", string command2 = "") override
	{
		erase(command1, command2);
		return;
	}
	
	void erase(string command1, string command2)
	{
		try
		{
			if (Validator::checkLBA(command1))
			{
				string command;
				int lba = stoi(command1), size = stoi(command2);

				changeLBAandSIZE(lba, size);


				int cycle = size / ERASE_BLOCK_SIZE;
				int cycleSize = ERASE_BLOCK_SIZE;
				int remainSize = size;

				for (int i = 0; i <= cycle; i++)
				{
					command = "ssd.exe E ";
					if (remainSize < ERASE_BLOCK_SIZE) cycleSize = remainSize;
					command = command + to_string(lba) + " " + to_string(cycleSize);
					eraseSSD(command);
					remainSize -= ERASE_BLOCK_SIZE;
				}
			}
		}
		catch (invalid_argument& e)
		{
			LOG("error message : " + string(e.what()) + "\n");
		}
	}

	void eraseSSD(string command)
	{
		SSDCommandExecutor::runCommand(command);
	}

	void changeLBAandSIZE(int& lba, int& size)
	{
		if (size < 0)
		{
			int end = lba;
			size = abs(size);
			lba = lba - size + 1;
			if (lba < 0)
			{
				lba = 0;
				size = end + 1;
			}
		}
		else if (lba + size -1 > MAX_LBA)
		{
			size = MAX_LBA - lba + 1;
		}
		
	}

};
class EraseRange : public Erase
{
public:

	void run(string command1 = "", string command2 = "") override
	{
		eraseRangeSSD(command1, command2);
		return;
	}

	void eraseRangeSSD(string command1, string command2)
	{
		try
		{
			if (Validator::checkLBA(command1) && Validator::checkLBA(command2))
			{
				int start = stoi(command1), end = stoi(command2);
				int size = abs(start - end) + 1;
				if (start > end) erase(command2, to_string(size));
				else erase(command1, to_string(size));
			}
		}
		catch (invalid_argument& e)
		{
			LOG("error message : " + string(e.what()) + "\n");
		}
	}

};

class Read : public ITestOperation
{
public:

    void run(string command1 = "",string command2 = "") override
    {
	    read(command1);
        return;
    }

	virtual string read(string address)
	{

		string result = "";

		try
		{
			if (Validator::checkLBA(address))
			{
				result = readSSD(address);
			}
		}
		catch (invalid_argument& e)
		{
			LOG("error message : " + string(e.what()) + "\n");
		}
		return result;
	}

	string readSSD(string address)
	{
		string result = "";
		string command = "ssd.exe R " + address;

		SSDCommandExecutor::runCommand(command);
		result = SSDCommandExecutor::runReadOutput("ssd_output.txt");
		
		if (address.size() == 1) address = "0" + address;
		LOG("[Read] LBA  " + address + " : " + result + "\n");
		return result;
	}

};

class FullRead : public Read {

public:

    void run(string command1 = "", string command2 = "") override
    {
		fullread();
		return;
    }

	void fullread()
	{
		for (int i = 0; i < FULL_READ_SIZE; i++)
		{
			string lba = to_string(i);
			read(lba);
		}
	}

};

class Help : public ITestOperation
{
public:
	void run(string command1 = "", string command2 = "") override
	{
		std::string help =
			"Team Approve : 고아라 이동건 이서영 이수용 이용한 한상민\n"
			"How to use CMD\n"
			"read (address) : (address)의 데이터를 읽음\n"
			"write (address) (data) : (address)에 (data)를 씀\n"
			"fullread : 모든 데이터를 읽음\n"
			"fullwrite (data) : 모든 주소에 (data)를 씀\n"
			"exit : 종료\n"
			"help : 도움말\n";
		LOG(help);
	}
};

class Write : public ITestOperation
{
public:
	void run(string command1 = "", string command2 = "") override
	{
		write(command1, command2);
	}

	void write(string address, string data)
	{
		try
		{
			Validator::checkLBA(address);
			Validator::checkHexData(data);
			writeSSD(address, data);
		}
		catch (invalid_argument& e)
		{
			LOG("error message : " + string(e.what()) + "\n");
		}
		return;
	}

	virtual void writeSSD(string address, string data)
	{
		const char* exePath = "ssd";
		const char* writeCmd = "W";

		std::string command = std::string("\"") + exePath + " " + writeCmd + " " + address + " " + data;
		SSDCommandExecutor::runCommand(command);
		return;
	}
private:

};

class FullWrite : public Write
{
public:

	void run(string command1 = "", string command2 = "") override
	{
		fullWrite(command1);
	}

	void fullWrite(string data)
	{
		try
		{
			Validator::checkHexData(data);
			for (int i = 0; i < 100; i++)
			{
				writeSSD(to_string(i), data);
			}
		}
		catch (invalid_argument& e)
		{
			LOG("error message : " + string(e.what()) + "\n");
		}
		return;
	}
};

