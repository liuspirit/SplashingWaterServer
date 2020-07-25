// SplashingWaterServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//using to ctrl+c  quit
#include< signal.h> 
#include< stdlib.h> 
#include< stdio.h> 

#include "HttpServer.h"
#include "MessageHandler.h"

#include "Database\MySQLOperator.h"
#include "IPManager.h"

#include "SingleAccountHandle.h"
#include "AccountSheetManager.h"
#include "AccountGenerator.h"
#include "UserSheetHandler.h"
int quit_flag = 0;
void handler(int sig)
{
	quit_flag = 1;
}

void testSingleAccount()
{
	//SingleAccountHandle handle("100001");

	/*
	vector<string> vec = handle.getAllFriendAccount();
	cout << "AllFriendAccout:" << endl;
	for (int i = 0; i<vec.size(); i++)
	{
		cout << vec[i] << endl;
	}
	cout << "end" << endl;
	
	cout << "AllFriendAccoutInfo:" << endl;
	cout << handle.getAllFriendsInfo() << endl;
	cout << "end" << endl;
	Log_Fatal((handle.getAllFriendsInfo()).c_str());
	

	string strAccount = "100002";
	cout<<handle.getFriendInfo(strAccount) << endl;
	Log_Fatal((handle.getFriendInfo(strAccount)).c_str());

	

	cout << "IP:" << handle.getIP(strAccount) << endl;

	vec = handle.getOnlineFriendAccount();

	
	cout << "OnlineFriendAccount:" << endl;
	for (int i = 0; i<vec.size(); i++)
	{
		cout << vec[i] << endl;
	}
	cout << "end" << endl;

	
	string strFriendAccount = "100003";
	string strName = "xia";
	string strState = "1";
	string strIP = "192.168.1.120";
	
	if (handle.isAccountExist(strFriendAccount))
	{
		cout << strFriendAccount << " is exist" << endl;
		handle.deleteFriend(strFriendAccount);
	}
	else
	{
		cout << strFriendAccount << " is not exist" << endl;
		//handle.addFriend(strFriendAccount, strName, strState, strIP);
	}
	
	string strAccount = "100002";
	string strIP = "192.168.1.103";
	handle.updateIP(strAccount, strIP);
	cout << "IP:" << handle.getIP(strAccount) << endl;
	strIP = "192.168.1.102";
	handle.updateIP(strAccount, strIP);
	cout << "IP:" << handle.getIP(strAccount) << endl;


	//state
	string strState = "1";
	handle.updateState(strAccount, strState);
	cout << handle.getFriendInfo(strAccount) << endl;
	*/
	
	/*
	string strAccount = "100002";
	string strIP = "192.168.1.102";
	string strState = "0";
	AccountSheetManager ams;
	ams.updateOneAccountIP(strAccount, strIP);
	ams.updateOneAccountState(strAccount, strState);
	strAccount = "100001";
	strState = "1";
	ams.updateOneAccountState(strAccount, strState);
	*/
	/*
	string strPassword = "222222";
	string strPhone = "1111111111";
	string strName = "liyshsklu";
	string strAccount = "100004";
	UserSheetHandler ush(strAccount);
	ush.addUser(strName, strPassword, strPhone);
	
	if (ush.IsAccountExist())
	{
		cout << ush.getPassword() << endl;
	}

	ush.deleteUser();

	if (!ush.IsAccountExist())
	{
		cout << "Not exist!" << endl;
	}
	*/
}

void test()
{
	SingleAccountHandle sAH("100001");
	vector<string> vecOnlineFriendIP = sAH.getOnlineFriendIP();

	for (int i = 0; i < vecOnlineFriendIP.size(); i++)
	{
		cout << vecOnlineFriendIP[i] << endl;
	}
	/*
	ILogicLayer *pLogicLayer = new IPManager();

	char cTargetIP[1024];
	memset(cTargetIP, 0, sizeof(cTargetIP));
	pLogicLayer->getAccountIP("100001", "100002", cTargetIP);
	cout << cTargetIP << endl;

	delete pLogicLayer;
	pLogicLayer = NULL;
	*/
}

//0x01 0x03 10 01 100001 21 0x05 100002  0x06 This is Client 0x02


int _tmain(int argc, _TCHAR* argv[])
{
	bool bFlag = true;
	//入口函数
	signal(SIGINT, handler);

	//SQL初始化
	MySQLOperator::getInstance();

	//create server
	HttpServer::getInstance();
	//功能->用例
	vector<string> vConnectPort;
	while (!quit_flag)       //ctrl c  quit
	{
		//查询ServerPort是否有收数据线程退出，如有则关闭
		if (bFlag)
		{
			test();
			bFlag = false;
		}
		vConnectPort = HttpServer::getInstance()->getConnectPort();
		for (int i = 0; i < vConnectPort.size(); i++)
		{
			if (HttpServer::getInstance()->getPortStatus(vConnectPort[i]) == RunStatus::Stop)
			{
				Sleep(2);
				HttpServer::getInstance()->closePortConnect(vConnectPort[i]);
			}
		}
	}

	HttpServer::DestoryInstance();
	MySQLOperator::DestoryInstance();
	return 0;
}

