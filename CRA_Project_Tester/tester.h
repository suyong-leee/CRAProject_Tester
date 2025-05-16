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
	void run(string command1 = "",string command2 = "")
	{
		cout << "read" << command1 << command2 <<endl;
	}
};