#pragma once

//7.遍历所有在线好友功能
#include "Database\MySQLOperator.h"
#include <vector>

class SingleAccountHandle
{
public:
	SingleAccountHandle();
	SingleAccountHandle(string strClientAccount);
	virtual ~SingleAccountHandle();

public:
	void setAccount(string &strAccount);

	//获取IP
	string getIP(string &strAccount);
	//
	vector<string> getAllIP();

	//获取全部好友信息(账号，昵称，状态)
	string getAllFriendsInfo();
	//获取特定好友信息(账号，昵称，状态)
	//string getFriendInfo(string &strFriend);
	vector<string> getFriendInfo(string &strFriend);

	//获取在线好友账户
	vector<string> getOnlineFriendAccount();
	//获取在线好友账户的IP
	vector<string> getOnlineFriendIP();
	//获取所有好友账户
	vector<string> getAllFriendAccount();


	//特定账户表中添加一行好友信息
	void addFriend(string &strFriendAccount, string &strName, string &strState,string strIP = "");
	//特定账户表中删除一行好友信息
	void deleteFriend(string &strFriendAccount);

	//更新账户IP
	void updateIP(string &strAccount, string &strIP);
	//更新好友状态
	void updateState(string &strAccount, string &strState);

	//查询账户存在
	bool isAccountExist(string &strAccount);
	
protected:
	//查询的字段   /   指定的账户
	string selectData(vector<string> &fieldVec, string strFriendAccount = "");

private:
	string m_strDbCAccount;
	string m_strAccount;
};

