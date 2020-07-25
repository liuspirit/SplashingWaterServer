#pragma once

#include "HttpThread.h"
#include "MessageHandler.h"

class HttpServerPort : public HttpThread
{
public:
	HttpServerPort(SOCKET &connectSocket, string strIPAddr);
	virtual ~HttpServerPort();

public:
	virtual void run();
	bool sendData(const char* pData);
	void closeSocket();

protected:
	int receiveData(char *pReceiveData, int nLen);

private:

	string m_strIPAddr;  //receiveData flag;
	int m_nReceiveLength;
	SOCKET m_connectSocket;
	MessageHandler m_MessageHandler;
	char m_cReceiveData[2048];
};

