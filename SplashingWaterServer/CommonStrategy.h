#pragma once
#include ".\Strategy\IStrategy.h"
#include "MessageHandler.h"

class CommonStrategy :
	public IStrategy
{
public:
	CommonStrategy(ParserResult &ParseredData);
	virtual ~CommonStrategy();

public:
	virtual void processMessage();
	virtual void sendData();

protected:
	int m_nEvent;
	string m_strData1;
	string m_strData2;
	string m_strSourceAccount;

private:
	CommonStrategy();
	void parserDataSegment();

private:
	ParserResult m_ParseredData;
};

