#pragma once
#include "CommonStrategy.h"
class StateStrategy :
	public CommonStrategy
{
public:
	StateStrategy(ParserResult &ParseredData);
	virtual ~StateStrategy();

	//override
public:
	virtual void processMessage();
	void sendStateData(string strIP);

};

