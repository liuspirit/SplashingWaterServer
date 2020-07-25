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
1.	���ݿͻ����˻��������ݿ�User����ȡ������
2.	У������
3a.���벻ͬ���������ݣ�����
3b.������ͬ������е��Ĳ�
4.�Ӻ����б���ȡ��������Ϣ
5.���ͺ�����Ϣ���ͻ���
6.��ȡ����IP��
7.�ж�IP�Ƿ�Ͳ���IP��ͬ
8a�������ͬ���򷵻أ����̽���
8b������ͬ�������IP�������ѱ���Լ����IP��ַ���и��£�
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
	//  ��¼��1:0��	0x06	������Ϣ��״̬
	m_nEvent = 3;
	char cMessagePackage[1024];
	MessageHandler::buildDataElement(cMessagePackage, m_strFriendInfo.c_str(), m_nEvent, "000000", m_strLoginFlag.c_str());
	Log_Info("do LogiecStrategy::sendData: sendIP= %s", m_strData1.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(m_strData1, cMessagePackage);

}

void LoginStrategy::updateState()
{
	//״̬����
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
