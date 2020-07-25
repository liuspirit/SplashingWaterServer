#pragma once
#include "CommonStrategy.h"
#include "MessageHandler.h"

class LoginStrategy :
	public CommonStrategy
{
public:
	LoginStrategy(ParserResult &ParseredData);
	virtual ~LoginStrategy();

public:
	virtual void processMessage();
	virtual void sendData();
	void updateState();
private:
	string m_strLoginFlag;//1,0
	string m_strFriendInfo;
};

