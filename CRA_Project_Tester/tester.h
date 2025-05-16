#pragma once
#include <iostream>
using namespace std;
class ITestOperation
{
public:
	virtual void run(string command) = 0;
};

//example
class Read : public ITestOperation
{
public:
	void run(string command)
	{
		cout << "read" << command << endl;
	}
};

class Write : public ITestOperation
{
public:
	void run(string command)
	{
		cout << "write" << command << endl;
	}
};

class SSDTest :public ITestOperation
{
public:

	// ITestOperation을(를) 통해 상속됨
	void run(string command) override;
private:
	void FullWriteAndReadCompare();
	void PartialLBAWrite();
	void WriteReadAging();
};