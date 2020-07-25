#pragma once
#include <string>
#include <vector>
using namespace std;

class StrTool
{
public:
	StrTool();
	~StrTool();
	static bool StrTool::SplitString(const string& strSource, vector<string>& vResult, char delimiters);

	static bool SplitString(const string& strSource, vector<string>& vResult, const string& delimiters);
	static int ToInt(string strSource);
	static string ToString(int nSource);

	static void replaceChar(string &strSource, char cData, string strData);
};

