#pragma once
#include <iostream>
using namespace std;
class ITestOperation
{
public:
	virtual string run(string command = "",string command2 = "") = 0;
};

//example
class Read : public ITestOperation
{
public:
	string run(string command1 = "",string command2 = "")
	{
		cout << "read" << command1 <<endl;
		return "hello read";
	}
};

