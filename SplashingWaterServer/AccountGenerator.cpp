#include "stdafx.h"
#include "AccountGenerator.h"
#include <stdlib.h>
#include <time.h>
#include "UserSheetHandler.h"
#include <windows.h>
AccountGenerator::AccountGenerator()
{
	m_strCreatedAccount = "";
}


AccountGenerator::~AccountGenerator()
{
}

string AccountGenerator::createAccount()
{
	Log_Info("do AccountGenerator::createAccount() begin");
	createData();
	while (UserSheetHandler::IsAccountExist(m_strCreatedAccount))
	{
		Sleep(1);
		createData();
	}
	Log_Info("do AccountGenerator::getAccount() new account = %s", m_strCreatedAccount.c_str());
	return m_strCreatedAccount;
}

void AccountGenerator::createData()
{
	Log_Info("do AccountGenerator::createData() begin");
	srand((int)time(NULL));
	string strResult("1", PASSWORD_LENGTH);
	for (int i = 0; i < PASSWORD_LENGTH; i++)
	{
		strResult[i] = (rand() % 10) + '0';
	}
	m_strCreatedAccount = strResult;
	Log_Info("do AccountGenerator::createData() end");
}