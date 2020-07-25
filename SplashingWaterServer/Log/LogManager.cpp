#include <windows.h>
#include <time.h>
#include "LogManager.h"
#include "FileOperation.h"
#include <string>

#define FILE_MAX_CAPACITY 1024*1024
LogManager * LogManager::m_pInstance = NULL;

LogManager::LogManager()
{
	m_CurrentLogLevel = Log_EVENT;
	m_strFilePath = "E:\\ServerLog\\";
	m_nLogOutputLevel = 3;
}


LogManager::~LogManager()
{
}
void LogManager::createCurrentDate(char *pCurrentDate)
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	memset(pCurrentDate, 0, sizeof(pCurrentDate));
	sprintf(pCurrentDate, "Log_%d%d%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}

void LogManager::createFile(const char *pLogPathFileName)
{
	ofstream creatFile(pLogPathFileName);
	if (creatFile)
	{
		creatFile.close();
	}
}
bool LogManager::judgeCurrentFileOverMax(const char *pLogPathFileName)
{
	FILE *fp = fopen(pLogPathFileName, "r");
	if (!fp)
		return true;
	fseek(fp, 0L, SEEK_END);
	int nSize = ftell(fp);
	fclose(fp);

	if (nSize < FILE_MAX_CAPACITY) //1024*1024  300
	{
		return false;
	}
	return true;
}

void LogManager::setCurrentPathFileName(char *cLogPathFileName, const char *cCurrentData,int nFileNum)
{
	memset(cLogPathFileName, 0, sizeof(cLogPathFileName));
	strcpy(cLogPathFileName, m_strFilePath.c_str());
	strcat(cLogPathFileName, cCurrentData);
	if (nFileNum != 0)
	{
		strcat(cLogPathFileName, "_");
		string strLogFileNum = std::to_string(nFileNum);
		strcat(cLogPathFileName, strLogFileNum.c_str());
	}
	strcat(cLogPathFileName, ".txt");
}
void LogManager::createFile()
{
	char cLogPathFileName[1024];
	char cCurrentDate[1024];

	createCurrentDate(cCurrentDate);

	int nFileNum = 0;
	setCurrentPathFileName(cLogPathFileName, cCurrentDate, nFileNum);
	
	while (1)
	{
		//判断当前文件是否存在
		fstream file;
		file.open(cLogPathFileName, ios::in);
		if (!file)
		{
			createFile(cLogPathFileName);
			break;
		}
		else
		{
			file.close();
		}

		//存在,小于1M
		if (!judgeCurrentFileOverMax(cLogPathFileName))
		{
			break;
		}
		
		nFileNum++;
		//设置新的文件名
		createCurrentDate(cCurrentDate);
		setCurrentPathFileName(cLogPathFileName, cCurrentDate, nFileNum);
	}
	m_FileOperation.setFilePath(cLogPathFileName);
}

void LogManager::writeFile(const char * pData)
{
	createFile();
	m_FileOperation.writeLine(pData);
}


void LogManager::changeToLogFormat(char *pTargetBuff, int nTargetLen, const char * pSourceData)
{
	char pLogLevel[256];
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	
	sprintf(pTargetBuff, "%d-%d-%d %d:%d:%d:%d ", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	
	getLogLevel(pLogLevel, m_CurrentLogLevel);
	strcat(pTargetBuff, pLogLevel);
	strcat(pTargetBuff, pSourceData);

}
void LogManager::getLogLevel(char *pDataBuff, LOGLEVEL logLevel)
{
	switch (logLevel)
	{
	case Log_FATAL:
		strcpy(pDataBuff, "FATAL");
		break;
	case Log_ERROR:
		strcpy(pDataBuff, "ERROR");
		break;
	case Log_DEBUG:
		strcpy(pDataBuff, "DEBUG");
		break;
	case Log_EVENT:
		strcpy(pDataBuff, "EVENT");
		break;
	default:
		break;
	}
	strcat(pDataBuff, " ");
}

void LogManager::LogData(const char *pData, LOGLEVEL logLevel)
{
	if (logLevel > m_nLogOutputLevel)
	{
		return;
	}
	char Buff[1024];
	memset(Buff, 0, sizeof(Buff));
	m_CurrentLogLevel = logLevel;
	changeToLogFormat(Buff, sizeof(Buff),pData);
	writeFile(Buff);
}