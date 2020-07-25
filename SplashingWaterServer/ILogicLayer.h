#pragma once

class ILogicLayer
{
public:
	virtual void getAccountIP(const char *pClientAccount, const char *pTargetAccount, char *pTargetAccountIP) = 0;
};
