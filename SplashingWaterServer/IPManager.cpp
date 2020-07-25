#include "stdafx.h"
#include "IPManager.h"
#include "Database\DatabaseInterface.h"
#include "Database\MySQLOperator.h"
IPManager::IPManager()
{
}


IPManager::~IPManager()
{
}

void IPManager::getAccountIP(const char *pClientAccount, const char *pTargetAccount, char *pTargetAccountIP)
{
	DatabaseInterface *pDatabase = new MySQLOperator("lsy", "888888", "lsytest");
	vector<string> fieldVec(1,"IP");

	string strTargetAccount = pTargetAccount;
	strTargetAccount = "FriendAccount = '" + strTargetAccount + "'";
	string strDbCAccount = pClientAccount;
	strDbCAccount = "Account_" + strDbCAccount;

	string strResult = pDatabase->selectData(strDbCAccount.c_str(), fieldVec, strTargetAccount.c_str());
	strcpy(pTargetAccountIP, strResult.c_str());
	delete pDatabase;
	pDatabase = NULL;
}

