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

class Help : public ITestOperation
{
public:
	void run(string command)
	{
		cout << "Team Approve : ��ƶ� �̵��� �̼��� �̼��� �̿��� �ѻ��" << endl;
		cout << "How to use CMD" << endl;
		cout << "read (address) : (address)�� �����͸� ����" << endl;
		cout << "write (address) (data) : (address)�� (data)�� ��"<< endl;
		cout << "fullread : ��� �����͸� ����" << endl;
		cout << "fullwrite (data) : ��� �ּҿ� (data)�� ��" << endl;
		cout << "exit : ����" << endl;
		cout << "help : ����" << endl;
	}
};