#pragma once
#include "CommonStrategy.h"

class RegisterStrategy :
	public CommonStrategy
{
public:
	RegisterStrategy(ParserResult &ParseredData);
	virtual ~RegisterStrategy();

public:
	virtual void processMessage();
	virtual void matchData();
	virtual void sendData();

private:
	string m_strNewAccount;
	string m_strPhone;
	string m_strName;
	string m_strPassword;
	string m_strIP;
};

