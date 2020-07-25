#pragma once
/*
1.日志创建
2.写入
3.删除文件
4.日志格式定义
日志属性：
格式，级别
时间： 级别：内容

5.日志级别定义
6.外部写入接口
*/
#include "FileOperation.h"
#include<stdarg.h>

enum LOGLEVEL{ Log_FATAL, Log_ERROR, Log_DEBUG, Log_EVENT };

class LogManager
{
public:
	virtual ~LogManager();

	static LogManager* getInstance(){
		if (m_pInstance == NULL)
		{
			m_pInstance = new LogManager();
		}
		return m_pInstance;
	}

	static void DestoryInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	void LogData(const char *pData, LOGLEVEL logLevel);
	
private:
	LogManager();

	void createFile();

	
	void changeToLogFormat(char *pTargetBuff, int nTargetLen, const char *pSourceData);
	void writeFile(const char * pData);
	void getLogLevel(char *pDataBuff, LOGLEVEL logLevel);

	void createFile(const char *pLogPathFileName);
	void createCurrentDate(char *pCurrentDate);
	void setCurrentPathFileName(char *pLogPathFileName, const char *pCurrentDate, int nFileNum);
	bool judgeCurrentFileOverMax(const char *pLogPathFileName);
	
private:

	string m_strFilePath;
	int m_nLogOutputLevel;
	LOGLEVEL m_CurrentLogLevel;

	FileOperation m_FileOperation;
	static LogManager *m_pInstance;
};


static void Log_Info(const char *pData, ...)
{
	char cAllLog[2048];
	va_list ap;
	va_start(ap, pData);
	int nLength = vsprintf_s(cAllLog, pData, ap);
	cAllLog[nLength] = 0;
	va_end(ap);
	LogManager::getInstance()->LogData(cAllLog, Log_EVENT);
}
/*
static void Log_Info(const char *pData)
{
	LogManager::getInstance()->LogData(pData, Log_EVENT);
}*/
static void Log_Debug(const char *pData)
{
	LogManager::getInstance()->LogData(pData, Log_DEBUG);
}
static void Log_Error(const char *pData)
{
	LogManager::getInstance()->LogData(pData, Log_ERROR);
}

static void Log_Fatal(const char *pData)
{
	LogManager::getInstance()->LogData(pData, Log_FATAL);
}

//const char *format,...
static void Log_Info_d(const char *format, ...)
{
	LogManager::getInstance()->LogData(format, Log_FATAL);
}



