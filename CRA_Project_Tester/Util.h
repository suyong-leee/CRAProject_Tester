#pragma once
#define UTIL_H
#ifdef UTIL_H
#include <string>
#include <sstream>
#include<iostream>
#include <iomanip> 
#include<fstream>
using namespace std;
string createRandomString(void);
bool CompareData(string  writeBuffer, string readBuffer);
bool ends_with(const std::string& str, const std::string& suffix);
void removeFile(std::string& filename);
void makeFile(std::string& filename);
#endif