/*
	it support 

*/


#pragma once
#include <string>
#include <map>
#include <vector>
#include "HttpServerPort.h"
#include "Observer\ObserverInterface.h"
#include "Observer\Subject.h"
using namespace std;

class HttpServer: public HttpThread
{
public:
	
	static HttpServer* getInstance(){
		if (m_pInstance == NULL)
		{
			m_pInstance = new HttpServer();
		}
		return m_pInstance;
	}

	static void DestoryInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}


	virtual ~HttpServer();

	//该线程用于处理客户端请求
	virtual void run();

	//用于服务器的初始化
	void buildServer(int nPort);

	////////////////////////////////////
	void sendPortData(string &strIPAddr, const char* pData);
	void closePortConnect(string &strIPAddr);
	void closePortThread(string &strIPAddr);
	void closeAllPortConnect();
	vector<string> getConnectPort();
	RunStatus getPortStatus(string &strIPAddr);
private:
	HttpServer();

	bool serverListening();

private:
	SOCKET m_ServerSocket;
	map<string, HttpServerPort*> m_ServerPortMap;//IP--Port

	static HttpServer* m_pInstance;
};

