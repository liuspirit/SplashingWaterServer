#pragma once
#include "ILogicLayer.h"
class IPManager :
	public ILogicLayer
{
public:
	IPManager();
	virtual ~IPManager();

public:
	virtual void getAccountIP(const char *pClientAccount, const char *pTargetAccount, char *pTargetAccountIP);


};

