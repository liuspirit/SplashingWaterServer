#include "stdafx.h"
#include "HttpServerPort.h"

HttpServerPort::HttpServerPort(SOCKET &connectSocket,string strIPAddr)
{
	m_connectSocket = connectSocket;
	m_strIPAddr = strIPAddr;
	start_Thread();
}


HttpServerPort::~HttpServerPort()
{
	closeSocket();
}


void HttpServerPort::run()
{

	if (!receiveData(m_cReceiveData, sizeof(m_cReceiveData)))
	{
		closeSocket();
		return;
	}
	
	if (strcmp(m_cReceiveData, "") != 0)
	{
		//sendData("This is Test!");
		Log_Info(m_cReceiveData);  //打印客户端信息
		m_MessageHandler.parserMessage(m_cReceiveData, m_nReceiveLength);
		Log_Info("m_MessageHandler parser end 1");
		memset(m_cReceiveData, 0, sizeof(m_cReceiveData));
	}	
}

bool HttpServerPort::sendData(const char* pData)
{
	Log_Info("HttpServerPort::sendData() start");
	int nDataSize = send(m_connectSocket, pData, strlen(pData) + 1, 0);
	if (SOCKET_ERROR == nDataSize)
	{
		Log_Info("HttpServerPort::sendData() : %s failed to send data ", m_strIPAddr.c_str());
		return false;
	}
	Log_Info("HttpServerPort::sendData() end ");
	return true;
}

int HttpServerPort::receiveData(char *pReceiveData, int nLen)
{
	memset(pReceiveData, 0, sizeof(pReceiveData));
	Log_Info("HttpServerPort::receiveData 1");
	m_nReceiveLength = recv(m_connectSocket, pReceiveData, nLen, 0);
	if (SOCKET_ERROR == m_nReceiveLength || 0 == m_nReceiveLength)
	{
		Log_Info("HttpServerPort::receiveData 2");
		return SOCKET_ERROR;
	}
	Log_Info("HttpServerPort::receiveData 3");
	return m_nReceiveLength;
}

void HttpServerPort::closeSocket()
{
	closesocket(m_connectSocket);
	end_Thread();
	Log_Info("HttpServerPort:: %s closeSocket", m_strIPAddr.c_str());
}


