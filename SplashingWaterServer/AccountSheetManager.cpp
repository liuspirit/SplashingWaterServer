#include "stdafx.h"
#include "AccountSheetManager.h"
#include "Database\MySQLOperator.h"

AccountSheetManager::AccountSheetManager()
{
}


AccountSheetManager::~AccountSheetManager()
{
}

void AccountSheetManager::createAccountTable(string &strAccount)
{
	Log_Info("do AccountSheetManager::createAccountTable, Table = %s", strAccount.c_str());
	string strRawData = "create table Account_" + strAccount;
	strRawData = strRawData + "(ID int(4) not null primary key auto_increment,IP char(20) not null,FriendAccount char(20) not null,Name char(20) not null,State int(4) default 0); ";
	MySQLOperator::getInstance()->rawExecute(strRawData.c_str());
}

void AccountSheetManager::updateOneAccountIP(string &strAccount, string &strIP)
{
	Log_Info("do AccountSheetManager::updateOneAccountIP");
	//更新账户的IP，包括其好友账户表中的该账户IP
	//更新客户端账户IP
	m_SingleAccount.setAccount(strAccount);
	m_SingleAccount.updateIP(strAccount, strIP);

	//更新好友账户中的IP
	vector<string> vecAFriendAccount = m_SingleAccount.getAllFriendAccount();
	for (int i = 0; i < vecAFriendAccount.size(); i++)
	{
		m_SingleAccount.setAccount(vecAFriendAccount[i]);
		m_SingleAccount.updateIP(strAccount, strIP);
	}
}

void AccountSheetManager::updateOneAccountState(string &strAccount, string &strState)
{
	Log_Info("do AccountSheetManager::updateOneAccountState");
	//更新客户端账户状态
	m_SingleAccount.setAccount(strAccount);
	m_SingleAccount.updateState(strAccount, strState);

	//更新好友账户中的状态
	vector<string> vecAFriendAccount = m_SingleAccount.getAllFriendAccount();
	//vector<string> vecAFriendAccount = m_SingleAccount.getOnlineFriendAccount();
	for (int i = 0; i < vecAFriendAccount.size(); i++)
	{
		m_SingleAccount.setAccount(vecAFriendAccount[i]);
		m_SingleAccount.updateState(strAccount, strState);
	}
}