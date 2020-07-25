#include "FileOperation.h"


FileOperation::FileOperation()
{
	
}


FileOperation::~FileOperation()
{
}

void FileOperation::setFilePath(const char * pFilePath)
{
	m_strFilePath = pFilePath;
}

//write char 数组 到文件末尾
bool FileOperation::writeLine(const char* strData)
{
	try
	{
		m_OutFile.open(m_strFilePath, ios::in | ios::app | ios::binary);
	}
	catch (exception ex)
	{
		return false;
	}
	
	m_OutFile << strData << endl;
	m_OutFile.close();
	return true;
}

/*
bool FileOperation::ReadLine(char* strData, int nLen)
{
	try
	{
		m_ReadFile.open(m_strFilePath, ios::out | ios::binary);
	}
	catch (exception ex)
	{
		return false;
	}
	while (!m_ReadFile.eof())
	{
		m_ReadFile.getline(strData, nLen);
	}
	m_ReadFile.close();
	return true;
}
*/

