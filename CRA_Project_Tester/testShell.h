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
class ITestOperation
{
public:
	virtual void run(string command = "", string command2 = "") = 0;

};
class Flush : public ITestOperation
{
public:

	void flush()
	{
		string command = "ssd.exe F";

		FILE* pipe = _popen(command.c_str(), "r");
		if (!pipe) {
			cout <<  "error Flush : cannot open pipe";
		}

		_pclose(pipe);

	}
	void run(string command1 = "", string command2 = "") override
	{
		flush();
	}
};



class Erase : public ITestOperation
{
public:
	bool checkLBA(string LBA)
	{
		if (LBA.size() > 2)
		{
			throw invalid_argument("세 자리 이상 불가.");
		}
		if (LBA.size() == 0)
		{
			throw invalid_argument("한 자리 이상 입력 필수");
		}
		for (int i = 0; i < LBA.size(); i++)
		{
			if (LBA[i] >= '0' && LBA[i] <= '9') continue;
			else throw invalid_argument("0 ~ 9 사이 수만 가능");
		}
		return true;
	}
	
	void eraseSSD(string command)
	{
		
		FILE* pipe = _popen(command.c_str(), "r");
		if (!pipe) {
			cout << "erase cmd error: " << command << endl;
		}
		_pclose(pipe);
		
		//cout << "Erase command is " << command << endl;
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
	void erase(string command1, string command2)
	{
		try
		{
		    if (checkLBA(command1))
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
	  	    cout << "error message : " << e.what() << endl;
		}
	}
	void run(string command1 = "", string command2 = "") override
	{
		erase(command1, command2);
		return;
	}

};
class EraseRange : public Erase
{
public:
	void run(string command1 = "", string command2 = "") override
	{
		try
		{
			if (checkLBA(command1) && checkLBA(command2))
			{
				int start = stoi(command1), end = stoi(command2);
				int size = abs(start - end) + 1;
				if (start > end) erase(command2, to_string(size));
				else erase(command1, to_string(size));
			}
		}
		catch (invalid_argument& e)
		{
			cout << "error message : " << e.what() << endl;
		}
		return;
	}

};
//example
class Read : public ITestOperation
{
public:
    bool checkLBA(string LBA)
    {

	    if (LBA.size() > 2)
	    {
		    throw invalid_argument("세 자리 이상 불가.");
	    }
	    if (LBA.size() == 0)
	    {
		    throw invalid_argument("한 자리 이상 입력 필수");
	    }
	    for (int i = 0; i < LBA.size(); i++)
	    {
		    if (LBA[i] >= '0' && LBA[i] <= '9') continue;
		    else throw invalid_argument("0 ~ 9 사이 수만 가능");
	    }
	    return true;
    }

    void run(string command1 = "",string command2 = "") override
    {
	    read(command1);
        return;
    }

    virtual string read(string address)
    {

		string result;

		try
		{
			if (checkLBA(address))
			{
				string command = "ssd.exe R " + address;

				FILE* pipe = _popen(command.c_str(), "r");
				if (!pipe) {
					return "error: cannot open pipe";
				}
				
				_pclose(pipe);

				ifstream file("ssd_output.txt");
				if (file.is_open()) {
					string line;
					while (getline(file, line)) {
						result += line ;  // 여러 줄 출력 가능성 고려
					}
					file.close();
				}
				else {
					result = "error: cannot open output file";
				}

				if (address.size() == 1) address = "0" + address;
				LOG("[Read] LBA  "+address+" : "+result+"\n");
			}
		}
		catch (invalid_argument& e)
		{
			LOG("error message : %s\n", e.what());
		}
	return result;
    }

};

class FullRead : public Read {

public:
    void run(string command1 = "", string command2 = "") override
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
			checkAddress(address);
			checkData(data);
			callSSD(address, data);
		}
		catch (invalid_argument& e)
		{
			LOG("error message : %s", e.what());
		}
		return;
	}

	bool checkAddress(string address)
	{
		if (address.size() > 2 || address.size() == 0)
		{
			throw invalid_argument("0~99 사이만 가능.");
		}
		for (int i = 0; i < address.size(); i++)
		{
			if (address[i] >= '0' && address[i] <= '9') continue;
			else throw invalid_argument("0 ~ 9사이 수만 가능");
		}
		return true;
	}

	bool checkData(string data)
	{
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

	virtual void callSSD(string address, string data)
	{
		const char* exePath = "ssd";
		const char* writeCmd = "W";

		std::string command = std::string("\"") + exePath + " " + writeCmd + " " + address + " " + data;
		int result = std::system(command.c_str());
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
			checkData(data);
			for (int i = 0; i < 100; i++)
			{
				callSSD(to_string(i), data);
			}
		}
		catch (invalid_argument& e)
		{
			cout << "error message : " << e.what() << endl;
		}
		return;
	}
};

