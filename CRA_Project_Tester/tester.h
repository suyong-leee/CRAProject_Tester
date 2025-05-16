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