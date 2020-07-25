#pragma once
#define PASSWORD_LENGTH 6
class AccountGenerator
{
public:
	AccountGenerator();
	~AccountGenerator();

	string createAccount();

protected:
	void createData();

private:
	string m_strCreatedAccount;
};

