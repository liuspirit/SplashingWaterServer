#include "stdafx.h"
#include "LoginStrategy.h"
#include "IPManager.h"
#include "MessageHandler.h"
#include "HttpServer.h"

#include "UserSheetHandler.h"
#include "SingleAccountHandle.h"
#include "AccountSheetManager.h"
#include "SmartPtr.h"
#include "StateStrategy.h"

LoginStrategy::LoginStrategy(ParserResult &ParseredData) :CommonStrategy(ParseredData)
{
	m_strLoginFlag = "0";
	m_strFriendInfo = "";
}


LoginStrategy::~LoginStrategy()
{
}
/*
1.	根据客户端账户，从数据库User表中取出密码
2.	校验密码
3a.密码不同，则发送数据，返回
3b.密码相同，则进行第四步
4.从好友列表中取出好友信息
5.发送好友信息给客户端
6.获取自我IP；
7.判断IP是否和参数IP相同
8a，如果相同，则返回，流程结束
8b，不相同，则更新IP，（好友表和自己表的IP地址进行更新）
*/
void LoginStrategy::processMessage()
{
	CommonStrategy::processMessage();
	UserSheetHandler userHandler(m_strSourceAccount);
	AccountSheetManager AccountManager;

	if (userHandler.getPassword() != m_strData2)
	{
		if (userHandler.getPassword() == "1")
		{
			m_strFriendInfo = "account is exist";
		}
		else
		{
			m_strFriendInfo = "Password is wrong";
		}
		m_strLoginFlag = "0";
		sendData();
		return;
	}
	
	SingleAccountHandle accountHandler(m_strSourceAccount);

	

	//update IP
	if (accountHandler.getIP(m_strSourceAccount) != m_strData1)
	{
		AccountManager.updateOneAccountIP(m_strSourceAccount, m_strData1);
	}
	updateState();

	m_strLoginFlag = "1";
	m_strFriendInfo = accountHandler.getAllFriendsInfo();
	sendData();
}

void LoginStrategy::sendData()
{
	//  登录（1:0）	0x06	好友信息和状态
	m_nEvent = 3;
	char cMessagePackage[1024];
	MessageHandler::buildDataElement(cMessagePackage, m_strFriendInfo.c_str(), m_nEvent, "000000", m_strLoginFlag.c_str());
	Log_Info("do LogiecStrategy::sendData: sendIP= %s", m_strData1.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(m_strData1, cMessagePackage);

}

void LoginStrategy::updateState()
{
	//状态策略
	Log_Info("LoginStrategy::updateState()  begin");

	ParserResult result;
	result.strSourceAccount = m_strSourceAccount;

	char cData2[20];
	memset(cData2, 0, sizeof(cData2));


	strcat(cData2, "1");
	char m_cDataInterval[2] = { Data_INTERVAL, '\0' };
	strcat(cData2, m_cDataInterval);
	strcat(cData2, m_strSourceAccount.c_str());
	result.strData = cData2;
	Log_Info("LoginStrategy::updateState() %s", cData2);
	SmartPtr<IStrategy> pStrategy = new StateStrategy(result);
	pStrategy->processMessage();

	Log_Info("LoginStrategy::updateState()  end");
}
