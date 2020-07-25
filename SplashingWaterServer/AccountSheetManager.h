//1.创建表
#include "SingleAccountHandle.h"
#pragma once
class AccountSheetManager
{
public:
	AccountSheetManager();
	virtual ~AccountSheetManager();


public:
	//
	void createAccountTable(string &strAccount);
	void deleteAccountTable(string &strAccount);

	//更新账户的IP，包括其好友账户表中的该账户IP
	void updateOneAccountIP(string &strAccount, string &strIP);

	//更新账户状态，包括好友账户中该账户状态
	void updateOneAccountState(string &strAccount, string &strState);
private:
	SingleAccountHandle m_SingleAccount;

};

