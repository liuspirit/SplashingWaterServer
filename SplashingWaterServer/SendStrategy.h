#pragma once
#include "CommonStrategy.h"
#include "MessageHandler.h"

class SendStrategy :
	public CommonStrategy
{
	//construction and deconstruction
public:
	SendStrategy(ParserResult &ParseredData);
	virtual ~SendStrategy();

	//override
public:
	virtual void processMessage();
	virtual void sendData();
	void readIPAddress();

private:
	string m_strIP;
	
};

