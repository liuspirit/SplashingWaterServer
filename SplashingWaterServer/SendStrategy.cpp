#include "stdafx.h"
#include "SendStrategy.h"
#include "MessageHandler.h"
#include "HttpServer.h"

#include "SingleAccountHandle.h"
SendStrategy::~SendStrategy()
{
	m_strIP = "";
}

SendStrategy::SendStrategy(ParserResult &ParseredData) :CommonStrategy(ParseredData)
{
	
}

// client    friend   message
void SendStrategy::readIPAddress()
{
	Log_Debug("do SendStrategy::readIPAddress");
	
	SingleAccountHandle singleHandle(m_strSourceAccount);
	m_strIP = singleHandle.getIP(m_strData1);
}

void SendStrategy::processMessage()
{
	Log_Info("SendStrategy::processMessage  begin");
	CommonStrategy::processMessage();
	readIPAddress();
	sendData();
	Log_Debug("SendStrategy::processMessage  end");
}

void SendStrategy::sendData()
{
	char cMessagePackage[1024];
	m_nEvent = 1;
	MessageHandler::buildDataElement(cMessagePackage, m_strData2.c_str(), m_nEvent, "000000", m_strSourceAccount.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(m_strIP, cMessagePackage);
}