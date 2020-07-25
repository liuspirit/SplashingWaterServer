#include "stdafx.h"
#include "CommonStrategy.h"
#include "HttpServer.h"

CommonStrategy::CommonStrategy(ParserResult &ParseredData)
{
	m_ParseredData = ParseredData;
}


CommonStrategy::~CommonStrategy()
{
}

void CommonStrategy::parserDataSegment()
{
	int nPosition = m_ParseredData.strData.find(0x06);
	m_strData1 = m_ParseredData.strData.substr(0, nPosition);
	m_strData2 = m_ParseredData.strData.substr(nPosition + 1);//message
	m_strSourceAccount = m_ParseredData.strSourceAccount;
}

void CommonStrategy::processMessage()
{
	parserDataSegment();
}

void CommonStrategy::sendData()
{

}