#pragma once
#include "testShell.h"

class SSDTest_FullWriteAndReadCompare :public ITestOperation
{
public:
	SSDTest_FullWriteAndReadCompare(Write* w, Read* r) : write(w), read(r) {};
	void run(string command1 = "", string command2 = "") override;
private:
	Write* write;
	Read* read;
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

class SSDTest_WriteReadAging :public ITestOperation, public exception
{
public:
	SSDTest_WriteReadAging(Write* w, Read* r) : mWrite(w), mRead(r) {};
	SSDTest_WriteReadAging() : mWrite(nullptr), mRead(nullptr) {};
	void run(string param1, string param2) override;

private:
	Write* mWrite;
	Read* mRead;
};

class SSDTest_EraseAndWriteAging :public ITestOperation, public exception
{
public:
	SSDTest_EraseAndWriteAging(Write* w, Read* r, Erase* e) : mWrite(w), mRead(r), mErase(e) {};
	SSDTest_EraseAndWriteAging() : mWrite(nullptr), mRead(nullptr), mErase(nullptr) {};
	void run(string param1, string param2) override;

private:
	Write* mWrite;
	Read* mRead;
	Erase* mErase;

	void WriteAndErase(int start_addr);
};

class SSDTest_FullScenario :public ITestOperation, public exception
{
public:
	void run(string file_name, string param2) override;
};