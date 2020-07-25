#pragma once

//7.�����������ߺ��ѹ���
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

	//��ȡIP
	string getIP(string &strAccount);
	//
	vector<string> getAllIP();

	//��ȡȫ��������Ϣ(�˺ţ��ǳƣ�״̬)
	string getAllFriendsInfo();
	//��ȡ�ض�������Ϣ(�˺ţ��ǳƣ�״̬)
	//string getFriendInfo(string &strFriend);
	vector<string> getFriendInfo(string &strFriend);

	//��ȡ���ߺ����˻�
	vector<string> getOnlineFriendAccount();
	//��ȡ���ߺ����˻���IP
	vector<string> getOnlineFriendIP();
	//��ȡ���к����˻�
	vector<string> getAllFriendAccount();


	//�ض��˻��������һ�к�����Ϣ
	void addFriend(string &strFriendAccount, string &strName, string &strState,string strIP = "");
	//�ض��˻�����ɾ��һ�к�����Ϣ
	void deleteFriend(string &strFriendAccount);

	//�����˻�IP
	void updateIP(string &strAccount, string &strIP);
	//���º���״̬
	void updateState(string &strAccount, string &strState);

	//��ѯ�˻�����
	bool isAccountExist(string &strAccount);
	
protected:
	//��ѯ���ֶ�   /   ָ�����˻�
	string selectData(vector<string> &fieldVec, string strFriendAccount = "");

private:
	string m_strDbCAccount;
	string m_strAccount;
};

