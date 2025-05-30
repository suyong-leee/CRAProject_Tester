#pragma once
#include "testScript.h"
#include "testShell.h"
#include "Logger.h"
class TestRun
{
public:
	enum operatorOrder
	{

		OPERATOR_WRITE = 0,
		OPERATOR_ERASE,
		OPERATOR_ERASE_RANGE,
		PARAM_TWO = OPERATOR_ERASE_RANGE,

		OPERATOR_FULLWRITE,
		OPERATOR_READ,
		PARAM_ONE = OPERATOR_READ,

		OPERATOR_FULLREAD,

		OPERATOR_EXIT,
		OPERATOR_HELP,
		OPERATOR_FLUSH,

		SCENARIO_1,
		SCENARIO_2,
		SCENARIO_3,
		SCENARIO_4,
		NUM_OF_OPERATOR,
		PARAM_ZERO = NUM_OF_OPERATOR,
	};
	TestRun()
	{
		operators[OPERATOR_READ] = new Read;
		operators[OPERATOR_FULLREAD] = new FullRead;
		operators[OPERATOR_HELP] = new Help;
		operators[OPERATOR_WRITE] = new Write;
		operators[OPERATOR_FULLWRITE] = new FullWrite;
		operators[OPERATOR_ERASE] = new Erase;
		operators[OPERATOR_ERASE_RANGE] = new EraseRange;
		operators[OPERATOR_FLUSH] = new Flush;

		operators[SCENARIO_1] = new SSDTest_FullWriteAndReadCompare;
		operators[SCENARIO_2] = new SSDTest_PartialLBAWrite;
		operators[SCENARIO_3] = new SSDTest_WriteReadAging;
		operators[SCENARIO_4] = new SSDTest_EraseAndWriteAging;
	}
	bool RunCommand()
	{

		int operationOrder = getOperationNumber(getInput());

		if (operationOrder == -1)
		{
			LOG("INVALID COMMAND\n");
			return true;
		}

		if (operationOrder == OPERATOR_EXIT)
		{
			return false;
		}

		getParameters(operationOrder);

		currentOperation = getOperator(operationOrder);

		try
		{
			currentOperation->run(parameter[0], parameter[1]);
			LOG("Pass\n");
		}
		catch(exception & e)
		{
			LOG("Fail\n");
		}

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
	ITestOperation* operators[NUM_OF_OPERATOR] = { nullptr, };
	string parameter[2] = { "", };

	int getOperationNumber(const string& command)
	{
		if (command == "read") return OPERATOR_READ;
		else if (command == "write") return OPERATOR_WRITE;
		else if (command == "help") return OPERATOR_HELP;
		else if (command == "exit") return OPERATOR_EXIT;
		else if (command == "fullwrite") return OPERATOR_FULLWRITE;
		else if (command == "fullread") return OPERATOR_FULLREAD;
		else if (command == "flush") return OPERATOR_FLUSH;
		else if (command == "erase") return OPERATOR_ERASE;
		else if (command == "erase_range") return OPERATOR_ERASE_RANGE;
		else if (command == "1_" || command == "1_FullWriteAndReadCompare") return SCENARIO_1;
		else if (command == "2_" || command == "2_PartialLBAWrite") return SCENARIO_2;
		else if (command == "3_" || command == "3_WriteReadAging") return SCENARIO_3;
		else if (command == "4_" || command == "4_EraseAndWriteAging") return SCENARIO_4;

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