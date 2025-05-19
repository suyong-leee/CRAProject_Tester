#pragma once
#include <iostream>
using namespace std;
class ITestOperation
{
public:
	virtual void run(string command = "",string command2 = "") = 0;
};

//example
class Read : public ITestOperation
{
public:

	virtual void run(string command1 = "",string command2 = "") override
	{
		cout << "read" << command1 <<endl;
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
