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
	void run(string command1 = "",string command2 = "") override
	{
		cout << "read" << command1 << command2 <<endl;
	}
	string read(string address)
	{
		return "error";
	}
};

class Write : public ITestOperation
{
public:
	void run(string command1 = "", string command2 = "") override
	{
		cout << "write" << command1 << endl;
	}
	void write(string address, string data)
	{
		return;
	}
};

class SSDTest :public ITestOperation
{
public:

	// ITestOperation을(를) 통해 상속됨
	void run(string command1 = "", string command2 = "") override;
private:
	void FullWriteAndReadCompare();
	void PartialLBAWrite();
	void WriteReadAging();
};