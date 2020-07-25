#include "stdafx.h"
#include "StateStrategy.h"
#include "HttpServer.h"
#include "AccountSheetManager.h"
#include "SingleAccountHandle.h"
StateStrategy::StateStrategy(ParserResult &ParseredData) :CommonStrategy(ParseredData)
{
}


StateStrategy::~StateStrategy()
{
}

/*
状态接口参数：账户，状态（在线，还是不在线）

1收到客户端离线消息后，遍历客户端账户表，搜索到友方账户
2分别给友方账户发送状态更新消息
3更新友方账户表中的客户端状态信息，更新客户端账户表的状态信息

*/
//xxxxxx(客户端)   1/0(m_strData1)    xxxxxx(客户端)(m_strData2)
void StateStrategy::processMessage()
{
	Log_Info("StateStrategy::processMessage()-----begin");
	CommonStrategy::processMessage();

	
	SingleAccountHandle sAH(m_strData2);

	
	if (m_strData1 == "0")
	{
		//如果是状态断联，则关闭端口
		string strIP = sAH.getIP(m_strData2);
		HttpServer::getInstance()->closePortThread(strIP);
		Log_Info("StateStrategy::processMessage()-----close Account: %s", strIP.c_str());
	}
	
	//更新数据库
	AccountSheetManager sheetManager;
	sheetManager.updateOneAccountState(m_strData2, m_strData1);

	//send 在线客户端
	vector<string> vOnlineFriendIP = sAH.getOnlineFriendIP();

	for (int i = 0; i < vOnlineFriendIP.size(); i++)
	{
		sendStateData(vOnlineFriendIP[i]);
	}
	Log_Info("StateStrategy::processMessage()-----end");
}

void StateStrategy::sendStateData(string strIP)
{
	Log_Info("StateStrategy::sendStateData()-----begin, IP = %s",strIP.c_str());
	m_nEvent = 2;
	char cMessagePackage[1024];
	MessageHandler::buildDataElement(cMessagePackage, m_strData2.c_str(), m_nEvent, "000000", m_strData1.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(strIP, cMessagePackage);
}
