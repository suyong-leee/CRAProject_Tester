#pragma once
#include <iostream>
#include <string>
using namespace std;
class ITestOperation
{
public:
	virtual void run(string command = "", string command2 = "") = 0;

};

//example
class Read : public ITestOperation
{
public:
    bool checkCMD(string command)
    {

	    if (command.size() > 2)
	    {
		    throw invalid_argument("세 자리 이상 불가.");
	    }
	    if (command.size() == 0)
	    {
		    throw invalid_argument("한 자리 이상 입력 필수");
	    }
	    for (int i = 0; i < command.size(); i++)
	    {
		    if (command[i] >= '0' && command[i] <= '9') continue;
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
			if (checkCMD(address))
			{
				string command = "ssd.exe R " + address;
				string result;

				FILE* pipe = _popen(command.c_str(), "r");
				if (!pipe) {
					return "error: cannot open pipe";
				}

				char buffer[128];
				while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
					result += buffer;
				}

				_pclose(pipe);

				if (address.size() == 1) address = "0" + address;
				cout << "[Read] LBA " << address << " : " << result << endl;
			}
		}
		catch (invalid_argument& e)
		{
			cout << "error message : " << e.what() << endl;
		}
	return "error";
    }

};

class FullRead : public Read {

public:
    void run(string command1 = "", string command2 = "") override
    {
        for (int i = 0; i < 100; i++)
        {
	    string lba = to_string(i);
	    string result = read(lba);
            if (lba.size() == 1) lba = "0" + lba;	
		cout << "[Read] LBA " << lba << " : " << result << endl;
	}
    }

};
class Help : public ITestOperation
{
public:
	void run(string command1 = "", string command2 = "") override
	{
		cout << "Team Approve : 고아라 이동건 이서영 이수용 이용한 한상민" << endl;
		cout << "How to use CMD" << endl;
		cout << "read (address) : (address)의 데이터를 읽음" << endl;
		cout << "write (address) (data) : (address)에 (data)를 씀" << endl;
		cout << "fullread : 모든 데이터를 읽음" << endl;
		cout << "fullwrite (data) : 모든 주소에 (data)를 씀" << endl;
		cout << "exit : 종료" << endl;
		cout << "help : 도움말" << endl;
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
			cout << "error message : " << e.what() << endl;
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

