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
  
	 virtual string read(string address)
	{
		return "error";
	}
};

class FullRead : public Read {

public:
	virtual void run(string command1 = "", string command2 = "") override
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

class TestRun
{
public:
	enum operatorOrder
	{
		
		OPERATOR_WRITE = 0,
		OPERATOR_FULLWRITE,
		PARAM_TWO = OPERATOR_FULLWRITE,

		OPERATOR_READ,
		PARAM_ONE = OPERATOR_READ,
		OPERATOR_FULLREAD,

		OPERATOR_EXIT,
		OPERATOR_HELP,

		SCENARIO_1,
		SCENARIO_2,
		SCENARIO_3,
		NUM_OF_OPERATOR,
		PARAM_ZERO = NUM_OF_OPERATOR,
	};
	TestRun()
	{
		operators[OPERATOR_READ] = new Read;
		operators[OPERATOR_FULLREAD] = new FullRead;
		operators[OPERATOR_HELP] = new Help;
	}
	bool RunCommand()
	{

		int operationOrder = getOperationNumber(getInput());

		if (operationOrder == -1)
		{
			cout << "INVALID COMMAND" << endl;
			return true;
		}
		
		if (operationOrder == OPERATOR_EXIT)
		{
			return false;
		}

		getParameters(operationOrder);

		currentOperation = getOperator(operationOrder);

		currentOperation->run(parameter[0], parameter[1]);
		
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
	ITestOperation* operators[NUM_OF_OPERATOR] = {nullptr,};
	string parameter[2] = {"",};

	int getOperationNumber(const string& command)
	{
		if (command == "read") return OPERATOR_READ;
		else if (command == "write") return OPERATOR_WRITE;
		else if (command == "help") return OPERATOR_HELP;
		else if (command == "exit") return OPERATOR_EXIT;
		else if (command == "fullwrite") return OPERATOR_FULLWRITE;
		else if (command == "fullread") return OPERATOR_FULLREAD;
		else if (command == "1_" || command == "1_FullWriteAndReadCompare") return SCENARIO_1;
		else if (command == "2_" || command == "2_PartialLBAWrite") return SCENARIO_2;
		else if (command == "3_" || command == "3_WriteReadAging") return SCENARIO_3;
		else return -1;
	}

	void getParameters(int operationOrder)
	{
		if (operationOrder <= PARAM_TWO)
		{
			parameter[0] = getInput();
			parameter[1] = getInput();
		}
		else if (operationOrder <= PARAM_ONE)
		{
			parameter[0] = getInput();
			parameter[1] = "";
		}
		else
		{
			parameter[0] = "";
			parameter[1] = "";
		}
		return;
	}
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

