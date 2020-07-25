#include "stdafx.h"
#include "SingleAccountHandle.h"
#include "Common\StrTool.h"
#include "Database\DatabaseInterface.h"
SingleAccountHandle::SingleAccountHandle()
{
	m_strDbCAccount = "Account_100001";
	m_strAccount = "100001";
}

SingleAccountHandle::SingleAccountHandle(string strClientAccount)
{
	m_strDbCAccount = "Account_" + strClientAccount;
	m_strAccount = strClientAccount;
}

void SingleAccountHandle::setAccount(string &strAccount)
{
	m_strDbCAccount = "Account_" + strAccount;
	m_strAccount = strAccount;
}

SingleAccountHandle::~SingleAccountHandle()
{
}


string SingleAccountHandle::getIP(string &strAccount)
{
	Log_Info("SingleAccountHandle::getIP:account = %s", strAccount.c_str());
	vector<string> fieldVec(1, "IP");

	return selectData(fieldVec, strAccount);
}
vector<string> SingleAccountHandle::getAllIP()
{
	Log_Info("SingleAccountHandle::getAllIP");
	vector<string> vResults;
	vector<string> fieldVec(1, "IP");

	string strResult = selectData(fieldVec);

	StrTool::SplitString(strResult, vResults, FIELD_INTERVAL);
	return vResults;
}

string SingleAccountHandle::getAllFriendsInfo()
{
	Log_Info("do SingleAccountHandle::getAllFriendsInfo");

	vector<string> fieldVec;
	fieldVec.push_back("FriendAccount");
	fieldVec.push_back("Name");
	fieldVec.push_back("State");

	return selectData(fieldVec);
}

vector<string> SingleAccountHandle::getFriendInfo(string &strFriend)
{
	Log_Info("do SingleAccountHandle::getFriendInfo");
	vector<string> vResults;

	vector<string> fieldVec;
	fieldVec.push_back("IP");
	fieldVec.push_back("FriendAccount");
	fieldVec.push_back("Name");
	fieldVec.push_back("State");
	
	string strResult = selectData(fieldVec, strFriend);

	StrTool::SplitString(strResult, vResults, FIELD_INTERVAL);
	return vResults;
}

void SingleAccountHandle::addFriend(string &strFriendAccount, string &strName, string &strState, string strIP)
{
	Log_Info("do SingleAccountHandle::addFriend");
	map<string, string> keyValueMap;
	keyValueMap.insert(pair<string, string>("FriendAccount", "'" + strFriendAccount + "'"));
	keyValueMap.insert(pair<string, string>("Name", "'" + strName + "'"));
	keyValueMap.insert(pair<string, string>("State", strState));
	if (strIP != "")
	{
		keyValueMap.insert(pair<string, string>("IP", "'" + strIP + "'"));
	}
	else
	{
		keyValueMap.insert(pair<string, string>("IP", "'192.168.1.1'"));
	}
	MySQLOperator::getInstance()->insertData(m_strDbCAccount.c_str(), keyValueMap);
}

void SingleAccountHandle::deleteFriend(string &strFriendAccount)
{
	Log_Info("do SingleAccountHandle::deleteFriend");
	map<string, string> keyValueMap;
	keyValueMap.insert(pair<string, string>("FriendAccount", "'" + strFriendAccount + "'"));
	MySQLOperator::getInstance()->deleteData(m_strDbCAccount.c_str(), keyValueMap);
}

void SingleAccountHandle::updateIP(string &strAccount, string &strIP)
{
	Log_Info("do SingleAccountHandle::updateIP");
	map<string, string> keyValueMap;
	map<string, string> keyValueConMap;
	keyValueMap.insert(pair<string, string>("IP", "'" + strIP + "'"));
	keyValueConMap.insert(pair<string, string>("FriendAccount", "'" + strAccount + "'"));
	MySQLOperator::getInstance()->updateData(m_strDbCAccount.c_str(), keyValueMap, keyValueConMap);
}

void  SingleAccountHandle::updateState(string &strAccount, string &strState)
{
	Log_Info("do SingleAccountHandle::updateState");
	map<string, string> keyValueMap;
	map<string, string> keyValueConMap;
	keyValueMap.insert(pair<string, string>("State", strState));
	keyValueConMap.insert(pair<string, string>("FriendAccount", "'" + strAccount + "'"));
	MySQLOperator::getInstance()->updateData(m_strDbCAccount.c_str(), keyValueMap, keyValueConMap);
}


vector<string> SingleAccountHandle::getOnlineFriendAccount()
{
	Log_Info("do SingleAccountHandle::getOnlineFriendAccount");
	vector<string> fieldVec;
	vector<string> results;
	fieldVec.push_back("FriendAccount");
	string strCondition = "State = 1";

	string strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec, strCondition.c_str());

	StrTool::SplitString(strResult, results, FIELD_INTERVAL);

	return results;
}

vector<string> SingleAccountHandle::getOnlineFriendIP()
{
	Log_Info("do SingleAccountHandle::getOnlineFriendIP()------begin");
	vector<string> fieldVec;
	vector<string> results;
	fieldVec.push_back("IP");
	string strCondition = "State = 1 and friendaccount <> '" + m_strAccount + "'";

	string strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec, strCondition.c_str());

	StrTool::SplitString(strResult, results, FIELD_INTERVAL);

	Log_Info("do SingleAccountHandle::getOnlineFriendIP()------result %s",strResult.c_str());
	Log_Info("do SingleAccountHandle::getOnlineFriendIP()------end");
	return results;
}

vector<string> SingleAccountHandle::getAllFriendAccount()
{
	Log_Info("do SingleAccountHandle::getAllFriendAccount");
	vector<string> fieldVec;
	vector<string> results;
	fieldVec.push_back("FriendAccount");
	string strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec);
	StrTool::SplitString(strResult, results, FIELD_INTERVAL);
	return results;
}

bool SingleAccountHandle::isAccountExist(string &strAccount)
{
	Log_Info("do SingleAccountHandle::isAccountExist");
	vector<string> fieldVec;
	vector<string> results;
	fieldVec.push_back("Name");

	string strCondition = "FriendAccount = '" + strAccount + "'";

	string strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec, strCondition.c_str());

	if (strResult != "")
	{
		return true;
	}
	else
	{
		return false;
	}
}

string SingleAccountHandle::selectData(vector<string> &fieldVec, string strFriendAccount)
{
	Log_Info("do SingleAccountHandle::selectData");
	string strResult = "";
	if (strFriendAccount != "")
	{
		strFriendAccount = "FriendAccount = '" + strFriendAccount + "'";
		strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec, strFriendAccount.c_str());
	}
	else
	{
		strResult = MySQLOperator::getInstance()->selectData(m_strDbCAccount.c_str(), fieldVec);
	}
	Log_Info("do SingleAccountHandle::selectData, Result = %s",strResult.c_str());
	return strResult;
}