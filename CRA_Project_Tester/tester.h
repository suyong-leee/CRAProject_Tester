#pragma once
#include <iostream>
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
        throw invalid_argument("3자리 이상 불가.");
    }
    for (int i = 0; i < command.size(); i++)
    {
        if (command[i] >= '0' && command[i] <= '9') continue;
        else throw invalid_argument("0 ~ 9사이 수만 가능");
    }
    return true;
}

  virtual void run(string command1 = "",string command2 = "") override
	{
        try
        {
            if (checkCMD(command1))
            {
				string result = read(command1);
				if (command1.size() == 1) command1 = "0" + command1;
                cout << "[Read] LBA " << command1 << " : " << result << endl;
            }
        }
        catch (invalid_argument& e)
        {
            cout << "error message : " << e.what() << endl;
        }

        return;
	}
  
	string read(string address)
	{
		return "error";
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
	virtual void run(string command1 = "", string command2 = "") override
	{
		cout << "write" << command1 << endl;
	}
	void write(string address, string data)
	{
		return;
	}
};



class SSDTest_FullWriteAndReadCompare :public ITestOperation
{
public:
	SSDTest_FullWriteAndReadCompare(Write* w, Read* r) : write(w), read(r) {};
	void run(string command1 = "", string command2 = "") override;
private:
	Write* write;
	Read* read;
};

class SSDTest_PartialLBAWrite :public ITestOperation, public exception
{
public:
	SSDTest_PartialLBAWrite(Write* w, Read* r) : mWrite(w), mRead(r) {};
	SSDTest_PartialLBAWrite() : mWrite(nullptr), mRead(nullptr) {};
	void run(string param1, string param2) override;

private:
	Write* mWrite;
	Read* mRead;
};