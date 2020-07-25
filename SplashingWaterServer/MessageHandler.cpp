#include "stdafx.h"
#include "MessageHandler.h"
#include "Log\LogManager.h"
#include "Common\StrTool.h"
#include "SendStrategy.h"
#include "LoginStrategy.h"
#include "StateStrategy.h"
#include "UpdatelistStrategy.h"
#include "RegisterStrategy.h"
#include "SmartPtr.h"

char MessageHandler::m_cDataHead[3] = { DataElementHead1, DataElementHead2, '\0' };
char MessageHandler::m_cEvent[3] = { '0', '1', '\0' };
char MessageHandler::m_cMeta_DataInterval[2] = { MetaData_Data__INTERVAL, '\0' };
char MessageHandler::m_cDataInterval[2] = { Data_INTERVAL, '\0' };
char MessageHandler::m_cDataElementTail[2] = { DataElementTail, '\0' };
char MessageHandler::m_cDataFriendInterval[2] = { FriendList_INTERVAL, '\0' };

MessageHandler::MessageHandler()
{
	
}


MessageHandler::~MessageHandler()
{
}


int MessageHandler::converterToInt(const char *pNum, int nLen)
{
	int nNum = 0;

	if (!pNum)
	{
		throw exception("converterToInt(): Source is Null");
	}
	if (!nLen)
	{
		throw exception("converterToInt(): Length must be more than 0");
	}

	int nIndex = 0;
	while (nLen--)
	{
		if (pNum[nLen] >= '0'&&pNum[nLen] <= '9')
		{
			nNum += (pNum[nLen] - '0')*pow(10, nIndex);
		}
		else
		{
			throw exception("converterToInt(): There is only number");
		}
		nIndex++;
	}
	return nNum;
}

void MessageHandler::parserMessage(const char *pSource, int nLength)
{
	Log_Debug("MessageHandler::parserMessage begin");
	if (!pSource)
	{
		Log_Fatal("MessageHandler::parser source  is null!");
		throw exception("MessageHandler::parser source  is null!");
	}
	if (strlen(pSource) != nLength -1)  //nLength contain \0
	{
		Log_Fatal("MessageHandler::parser source length is wrong!");
		throw exception("MessageHandler::parser source length is wrong!");
	}
	//依次判断消息是否完整,并提取数据
	//循环并记录相关标志位
	//0x01 0x03 xx 01 xxxxxx xxx 0x05 xxxx 0x06 xxxx 0x02
	int index = 0;
	int nFlag = 0;
	char cMetaData[100] = "";
	char cData[1024] = "";
	int i = 0, j = 0;

	while (index < nLength)
	{
		if (pSource[index] == DataElementHead2)
		{
			nFlag++;
		}
		else if (pSource[index] == MetaData_Data__INTERVAL)
		{
			nFlag++;
		}
		else if (pSource[index] == DataElementTail)
		{
			nFlag++;
		}

		if (nFlag == 1)
		{
			cMetaData[i++] = pSource[index + 1];
		}
		else if (nFlag == 2)
		{
			cData[j++] = pSource[index + 1];
		}
		index++;
	}

	if (nFlag != 3)
	{
		Log_Fatal("MessageHandler::pSource not complete!");
		throw exception("MessageHandler::pSource not complete!");
	}

	cMetaData[--i] = '\0';
	cData[--j] = '\0';

	if (converterToInt(cMetaData, 2) != strlen(cMetaData))
	{
		Log_Fatal("MessageHandler::metaData not complete!");
		throw exception("MessageHandler::metaData not complete!");
	}

	if (converterToInt(cMetaData + 10, strlen(cMetaData) - 10) != strlen(cData))
	{
		Log_Fatal("MessageHandler::Data not complete!");
		throw exception("MessageHandler::Data not complete!");
	}

	m_ParserResult.strData = cData;
	processMetaData(cMetaData);

	Log_Debug("MessageHandler::parserMessage end");
}

void MessageHandler::processMetaData(string strSource)
{
	Log_Debug("MessageHandler::processMetaData begin");
	m_ParserResult.strSourceAccount = strSource.substr(4, 6);

	if (strSource[3] == '1')
	{
		Log_Debug("MessageHandler::processMetaData -- choose Send Strategy");
		SmartPtr<IStrategy> pStrategy = new SendStrategy(m_ParserResult);
		pStrategy->processMessage();
		
	}
	else if (strSource[3] == '2')
	{
		Log_Debug("MessageHandler::processMetaData -- choose State Strategy");
		SmartPtr<IStrategy> pStrategy = new StateStrategy(m_ParserResult);
		pStrategy->processMessage();

	}
	else if (strSource[3] == '3')
	{
		Log_Debug("MessageHandler::processMetaData -- choose Logic Strategy");
		SmartPtr<IStrategy> pStrategy = new LoginStrategy(m_ParserResult);
		pStrategy->processMessage();
	}
	else if (strSource[3] == '4')
	{
		Log_Debug("MessageHandler::processMetaData -- choose Update Strategy");
		SmartPtr<IStrategy> pStrategy = new UpdatelistStrategy(m_ParserResult);
		pStrategy->processMessage();
	}
	else if (strSource[3] == '5')
	{
		Log_Debug("MessageHandler::processMetaData -- choose Register Strategy");
		//Account    IP 0x06 Password
		SmartPtr<IStrategy> pStrategy = new RegisterStrategy(m_ParserResult);
		pStrategy->processMessage();
	}

	Log_Debug("MessageHandler::processMetaData end");
}


//0x01 0x03 xx 01 100001 xxx 0x05 100002 0x06 This is Client 0x02
void MessageHandler::buildDataElement(char *pMessagePackage, const char *pMessage, int &nEvent, const char *pClientAccount, const char *pFriendAccount)
{
	Log_Debug("MessageHandler::buildDataElement begin");
	int nDataSize = strlen(pFriendAccount) + strlen(pMessage) + 1;

	string strDataSize = StrTool::ToString(nDataSize);

	int nMetaSize = 4 + strlen(pClientAccount) + strDataSize.size();
	string strMetaDataSize = StrTool::ToString(nMetaSize);

	m_cEvent[1] = '0' + nEvent;
	strcpy(pMessagePackage, m_cDataHead);
	strcat(pMessagePackage, strMetaDataSize.c_str());
	strcat(pMessagePackage, m_cEvent);
	strcat(pMessagePackage, pClientAccount);
	strcat(pMessagePackage, strDataSize.c_str());
	strcat(pMessagePackage, m_cMeta_DataInterval);
	strcat(pMessagePackage, pFriendAccount);
	strcat(pMessagePackage, m_cDataInterval);
	strcat(pMessagePackage, pMessage);
	strcat(pMessagePackage, m_cDataElementTail);

	Log_Debug("MessageHandler::buildDataElement end");
}

void MessageHandler::buildFriendElement(char *pMessagePackage, const char *pDataName, const char *pDataAccount, const char *pDataStatue)
{
	if (strcmp(pMessagePackage, "") != 0)//不等于空
	{
		strcat(pMessagePackage, m_cDataFriendInterval);
	}
	strcat(pMessagePackage, pDataName);
	strcat(pMessagePackage, m_cDataFriendInterval);
	strcat(pMessagePackage, pDataAccount);
	strcat(pMessagePackage, m_cDataFriendInterval);
	strcat(pMessagePackage, pDataStatue);
}

void MessageHandler::buildElement(char *pMessagePackage, vector<string> &vStrData)
{
	if (vStrData.size() <= 0)
	{
		throw exception("Data is null");
	}

	strcat(pMessagePackage, vStrData[0].c_str());
	for (unsigned int i = 1; i < vStrData.size(); i++)
	{
		strcat(pMessagePackage, m_cDataFriendInterval);
		strcat(pMessagePackage, vStrData[i].c_str());
	}
}


void MessageHandler::test()
{
	char cMessage[1024];
	int nEvent = 1;
	buildDataElement(cMessage, "This is Client", nEvent, "100001", "100002");
	parserMessage(cMessage, strlen(cMessage));
}



/*
void MessageHandler::parserMessage(const char *pSource, int nLength)
{
if (!pSource)
{
throw exception("MessageHandler::parser source  is null!");
}
string strSource = pSource;

if (strSource.size() != nLength)
{
throw exception("MessageHandler::parser source length is wrong!");
}



//判断消息是否完整


int n3Position = strSource.find(DataElementHead2);
int n5Position = strSource.find(MetaData_Data__INTERVAL);
int n2Position = strSource.find(DataElementTail);

if (n3Position == strSource.npos || n5Position == strSource.npos || n2Position == strSource.npos)
{
throw exception("MessageHandler::parser source length is not complete!");
}

//metaData length DataElement
int nMetaSize = StrTool::ToInt(strSource.substr(n3Position + 1, 2));
string strMetaData = strSource.substr(n3Position + 1, (n5Position - n3Position - 1));

if (nMetaSize != strMetaData.size())
{
throw exception("MessageHandler::parser MetaData Size length is not complete!");
}
int nSize = StrTool::ToInt(strSource.substr(n3Position + 11));
string strData = strSource.substr(n5Position + 1, (n2Position - n5Position - 1));
if (nSize != strData.size())
{
throw exception("MessageHandler::parser Data Size length is not complete!");
}

m_ParserResult.strData = strData;
processMetaData(strMetaData);//xx 01 100001 xxx
}
*/