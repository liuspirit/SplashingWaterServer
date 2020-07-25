#include "stdafx.h"
#include "RegisterStrategy.h"
#include "MessageHandler.h"
#include "SingleAccountHandle.h"
#include "UserSheetHandler.h"
#include "Common\StrTool.h"
#include "AccountSheetManager.h"
#include "AccountGenerator.h"
#include "HttpServer.h"
RegisterStrategy::RegisterStrategy(ParserResult &ParseredData) :CommonStrategy(ParseredData)
{
	
}


RegisterStrategy::~RegisterStrategy()
{
}
/*
ע��ӿڲ�����Name���绰������

1.	���ݵ绰�������ݿ�User������ѡ���Ƿ�������
2a��������ڣ���ע��ʧ��
2b����������ڣ���ע��ɹ���
3��ע��ɹ���ͨ���˺������������˺ţ������ӵ�User�У�
4��	���͸��ͻ����˺ţ�
5�������˻�������¼һ���˺ţ�IPΪ��192.168.1.1��

*/
void RegisterStrategy::matchData()
{
	Log_Info("do  RegisterStrategy::matchData()  begin");
	vector<string> vResults;
	StrTool::SplitString(m_strData2, vResults, FriendList_INTERVAL);

	if (vResults.size() != 3)
	{
		throw exception("register information is not full");
	}
	m_strName = m_strData1;
	m_strPhone = vResults[2];
	m_strPassword = vResults[1];
	m_strIP = vResults[0];
	Log_Info("do  RegisterStrategy::matchData()  end");
}

void RegisterStrategy::processMessage()
{
	CommonStrategy::processMessage();
	matchData();
	Log_Info("do  processMessage::IsPhoneRegister()  begin");
	if (UserSheetHandler::IsPhoneRegister(m_strPhone))
	{
		Log_Error("RegisterStrategy::processMessage---- This Phone is already registered");
		m_strNewAccount = "xxxxxx";
		sendData();
		return;
	}
	Log_Info("do  processMessage::createAccount()  begin");
	AccountGenerator  ag;
	m_strNewAccount = ag.createAccount();

	Log_Info("do  processMessage::addUser()  begin");
	UserSheetHandler user(m_strNewAccount);
	user.addUser(m_strName, m_strPassword, m_strPhone);

	Log_Info("do  processMessage::createAccountTable()  begin");
	AccountSheetManager sheetMger;
	sheetMger.createAccountTable(m_strNewAccount);
	
	Log_Info("do  processMessage::addFriend()  begin");
	SingleAccountHandle sah(m_strNewAccount);
	string strState = "1";
	sah.addFriend(m_strNewAccount, m_strName, strState, m_strIP);

	sendData();
}

void RegisterStrategy::sendData()
{
	m_nEvent = 5;
	char cMessagePackage[1024];
	MessageHandler::buildDataElement(cMessagePackage, m_strNewAccount.c_str(), m_nEvent, "000000", m_strNewAccount.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(m_strIP, cMessagePackage);
}