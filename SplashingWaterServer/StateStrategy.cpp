#include "stdafx.h"
#include "StateStrategy.h"
#include "HttpServer.h"
#include "AccountSheetManager.h"
#include "SingleAccountHandle.h"
StateStrategy::StateStrategy(ParserResult &ParseredData) :CommonStrategy(ParseredData)
{
}


StateStrategy::~StateStrategy()
{
}

/*
״̬�ӿڲ������˻���״̬�����ߣ����ǲ����ߣ�

1�յ��ͻ���������Ϣ�󣬱����ͻ����˻����������ѷ��˻�
2�ֱ���ѷ��˻�����״̬������Ϣ
3�����ѷ��˻����еĿͻ���״̬��Ϣ�����¿ͻ����˻����״̬��Ϣ

*/
//xxxxxx(�ͻ���)   1/0(m_strData1)    xxxxxx(�ͻ���)(m_strData2)
void StateStrategy::processMessage()
{
	Log_Info("StateStrategy::processMessage()-----begin");
	CommonStrategy::processMessage();

	
	SingleAccountHandle sAH(m_strData2);

	
	if (m_strData1 == "0")
	{
		//�����״̬��������رն˿�
		string strIP = sAH.getIP(m_strData2);
		HttpServer::getInstance()->closePortThread(strIP);
		Log_Info("StateStrategy::processMessage()-----close Account: %s", strIP.c_str());
	}
	
	//�������ݿ�
	AccountSheetManager sheetManager;
	sheetManager.updateOneAccountState(m_strData2, m_strData1);

	//send ���߿ͻ���
	vector<string> vOnlineFriendIP = sAH.getOnlineFriendIP();

	for (int i = 0; i < vOnlineFriendIP.size(); i++)
	{
		sendStateData(vOnlineFriendIP[i]);
	}
	Log_Info("StateStrategy::processMessage()-----end");
}

void StateStrategy::sendStateData(string strIP)
{
	Log_Info("StateStrategy::sendStateData()-----begin, IP = %s",strIP.c_str());
	m_nEvent = 2;
	char cMessagePackage[1024];
	MessageHandler::buildDataElement(cMessagePackage, m_strData2.c_str(), m_nEvent, "000000", m_strData1.c_str());
	Log_Debug(cMessagePackage);
	HttpServer::getInstance()->sendPortData(strIP, cMessagePackage);
}
