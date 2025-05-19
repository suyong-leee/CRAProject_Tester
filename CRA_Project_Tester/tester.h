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
		cout << "read" << command1 <<endl;
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

class TestRun
{
public:
	enum operatorOrder
	{
		OPERATOR_WRTIE = 0,
		OPERATOR_READ,
		OPERATOR_EXIT,
		OPERATOR_HELP,
		OPERATOR_FULLWRITE,
		OPERATOR_FULLREAD,

		SCENARIO_1,
		SCENARIO_2,
		SCENARIO_3,
	};
	TestRun()
	{
		operators[OPERATOR_READ] = new Read;
		operators[OPERATOR_HELP] = new Help;
	}
	bool RunCommand()
	{
		string command, param1, param2;

		command = getInput();
		param1 = "";
		param2 = "";
		cout << command << endl;
		if (command == "read") 
		{
			param1 = getInput();
			currentOperation = getOperator(OPERATOR_READ);
		}
		else if (command == "help")
		{
			currentOperation = getOperator(OPERATOR_HELP);
		}
		else if (command == "exit")
		{
			return false;
		}
		else
		{
			cout << "INVALID COMMAND" << endl;
		}
		currentOperation->run(param1, param2);
		return true;
	}

	virtual string getInput() {
		string value;
		cin >> value;
		return value;
	}
	
	virtual ITestOperation* getOperator(int operation)
	{
		return operators[operation];
	}
private:
	ITestOperation* currentOperation = nullptr;
	ITestOperation* operators[10] = {nullptr,};
};
