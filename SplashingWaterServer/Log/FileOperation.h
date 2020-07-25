#pragma once
#include<fstream>
#include<iostream>
using namespace std;
class FileOperation
{
public:
	FileOperation();
	virtual ~FileOperation();
	
	void setFilePath(const char * strFilePath);
	bool writeLine(const char* strData);
	//bool ReadLine(char* strData, int nLen);

private:
	ofstream m_OutFile;
	ifstream m_ReadFile;
	string m_strFilePath;
};

