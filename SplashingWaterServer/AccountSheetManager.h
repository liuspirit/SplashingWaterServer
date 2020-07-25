//1.������
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

	//�����˻���IP������������˻����еĸ��˻�IP
	void updateOneAccountIP(string &strAccount, string &strIP);

	//�����˻�״̬�����������˻��и��˻�״̬
	void updateOneAccountState(string &strAccount, string &strState);
private:
	SingleAccountHandle m_SingleAccount;

};

