#include "StrTool.h"
#include <sstream>

StrTool::StrTool()
{
}


StrTool::~StrTool()
{
}
/***
void ReplaceBlank(char str[], int const size)
{

	if (NULL == str || size < 0)
	{
		return;
	}


	int orignalLength = 0;
	int numberOfBlank = 0;
	int i = 0;
	while (NUL != str[i])
	{
		++orignalLength;
		if (' ' == str[i])
		{
			++numberOfBlank;
		}
		++i;
	}
	++orignalLength;//加上结束符才是字符数组当前元素的个数。


	int newLength = orignalLength + 2 * numberOfBlank;


	if (newLength > size)
	{
		return;
	}

	int indexOfOrignal = orignalLength - 1;
	int indexOfNew = newLength - 1;
	while (indexOfOrignal >= 0 && indexOfNew > indexOfOrignal)
	{
		if (' ' == str[indexOfOrignal])
		{
			str[indexOfNew--] = '0';
			str[indexOfNew--] = '2';
			str[indexOfNew--] = '%';
		}
		else
		{
			str[indexOfNew--] = str[indexOfOrignal];
		}
		--indexOfOrignal;
	}
}

int main()
{
	char str[SIZE] = { "Iam very happy." };
	printf("源字符串：%s\n", str);
	ReplaceBlank(str, SIZE);
	printf("替换后的字符串：%s\n", str);
	return 0;

}
***/
void StrTool::replaceChar(string &strSource, char cData, string strData)
{
	while (true)   
	{
		string::size_type   pos(0);
		if ((pos = strSource.find(cData)) != string::npos)
			strSource.replace(pos, 1, strData);
		else   
			break;
	}
}

bool StrTool::SplitString(const string& strSource, vector<string>& vResult, char delimiters)
{
	string::size_type pos1, pos2;
	pos2 = strSource.find(delimiters);
	pos1 = 0;
	while (string::npos != pos2)
	{
		vResult.push_back(strSource.substr(pos1, pos2 - pos1));

		pos1 = pos2 + 1;
		pos2 = strSource.find(delimiters, pos1);
	}

	if (pos1 != strSource.length())
	{
		vResult.push_back(strSource.substr(pos1));
		return true;
	}
	else
	{
		return false;
	}
}
bool StrTool::SplitString(const string& strSource, vector<string>& vResult, const string& delimiters)
{
	string::size_type pos1, pos2;
	pos2 = strSource.find(delimiters);
	pos1 = 0;
	while (string::npos != pos2)
	{
		vResult.push_back(strSource.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delimiters.size();
		pos2 = strSource.find(delimiters, pos1);
	}

	if (pos1 != strSource.length())
	{
		vResult.push_back(strSource.substr(pos1));
		return true;
	}
	else
	{
		return false;
	}
		
}
int StrTool::ToInt(string strSource)
{
	return stoi(strSource);
}

string StrTool::ToString(int nSource)
{
	stringstream ss;
	ss << nSource;
	return ss.str();
}