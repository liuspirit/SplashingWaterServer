#include "stdafx.h"
#include "HttpServer.h"
//#include "Converter.h"
#pragma comment(lib,"ws2_32.lib")


HttpServer* HttpServer::m_pInstance = NULL;
HttpServer::HttpServer()
{
	buildServer(5120);
}

HttpServer::~HttpServer()
{
	Terminate_Thread();
	closesocket(m_ServerSocket);
	WSACleanup();
}

void HttpServer::buildServer(int nIPPort)
{
	WSADATA  wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData); //调用Winsock2.2版本 0  -1

	SOCKADDR_IN serverAddr;
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(nIPPort);

	//绑定监听端口;当调用socket函数创建套接字后，该套接字并没有与本机地址和端口等信息相连，bind函数将完成这些工作
	bind(m_ServerSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	// 开始监听，指定最大同时连接数为2
	listen(m_ServerSocket, 2);
	start_Thread();

	Log_Info("HttpServer::buildServer()------Create Server successfully");
}

void HttpServer::run()
{
	serverListening();
}

bool HttpServer::serverListening()
{
	Log_Info("HttpServer::serverListening()------Server is in listening");
	
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	SOCKET connectSocket = accept(m_ServerSocket, (SOCKADDR *)&clientAddr, &len);

	if (connectSocket == INVALID_SOCKET)//INVALID_SOCKET非法套接字
	{
		Log_Error("HttpServer::serverListening()------Server Accept Client Connect Fail");
		return false;
	}

	string strIP = inet_ntoa(clientAddr.sin_addr);

	try
	{
		HttpServerPort *pServerPort = new HttpServerPort(connectSocket, strIP);

		m_ServerPortMap.insert(pair<string, HttpServerPort*>(strIP, pServerPort));

		char data[2048] = "HttpServer::serverListening()------new client is connected! IP :";
		strcat(data, strIP.c_str());
		Log_Info(data);
	}
	catch (const exception &ex)
	{
		string str = ex.what();
		Log_Info("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  %s", str.c_str());
	}
	
	return true;
}

void HttpServer::sendPortData(string &strIPAddr, const char* pData)
{
	if (0 == m_ServerPortMap.count(strIPAddr))
	{
		string strAddr = "HttpServer::sendData()------This '" + strIPAddr + "' is not exist";
		Log_Error(strAddr.c_str());
		return;
	}
	m_ServerPortMap.at(strIPAddr)->sendData(pData);
}

void HttpServer::closePortConnect(string &strIPAddr)
{
	if (0 == m_ServerPortMap.count(strIPAddr))
	{
		string strAddr = "HttpServer::closePortConnect()------This '" + strIPAddr + "' is not exist";
		Log_Error(strAddr.c_str());
		return;
	}
	
	delete m_ServerPortMap.at(strIPAddr);
	m_ServerPortMap.at(strIPAddr) = NULL;
	m_ServerPortMap.erase(strIPAddr);
}

void HttpServer::closePortThread(string &strIPAddr)
{
	if (0 == m_ServerPortMap.count(strIPAddr))
	{
		string strAddr = "HttpServer::closePortThread()------This '" + strIPAddr + "' is not exist";
		Log_Error(strAddr.c_str());
		return;
	}
	m_ServerPortMap.at(strIPAddr)->end_Thread();
}

void HttpServer::closeAllPortConnect()
{
	map<string, HttpServerPort*>::iterator iter;
	for (iter = m_ServerPortMap.begin(); iter != m_ServerPortMap.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}
	m_ServerPortMap.clear();
}

vector<string> HttpServer::getConnectPort()
{
	vector<string> vPort;
	map<string, HttpServerPort*>::iterator iter;
	for (iter = m_ServerPortMap.begin(); iter != m_ServerPortMap.end(); iter++)
	{
		vPort.push_back(iter->first);
	}
	return vPort;
}

RunStatus HttpServer::getPortStatus(string &strIPAddr)
{
	if (0 == m_ServerPortMap.count(strIPAddr))
	{
		string strAddr = "HttpServer::getPortStatus()------This '" + strIPAddr + "' is not exist";
		Log_Error(strAddr.c_str());
		return RunStatus::Stop;
	}
	return m_ServerPortMap.at(strIPAddr)->getThreadStatus();
}