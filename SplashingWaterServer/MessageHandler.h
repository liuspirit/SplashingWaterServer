#pragma once
/*
�������ݰ�(�ͻ�������)��

�ͻ������ݵ����Ժ󣬵���parserMessage��
���Խ�����Ԫ���ݺ�������
ͨ����������������ɻ�ȡ

�ϳ����ݰ�:
*/
#include <vector>
#define DataElementHead1 0x01
#define DataElementHead2 0x03
#define MetaData_Data__INTERVAL 0x05
#define Data_INTERVAL 0x06
#define DataElementTail 0x02
#define FriendList_INTERVAL 0x10

struct ParserResult
{
	string strSourceAccount;
	string strData;
};


class MessageHandler
{
public:
	MessageHandler();
	virtual ~MessageHandler();


public:
	void parserMessage(const char *pSource, int nLength);
	//void parserMessage1(const char *pSource, int nLength);
	ParserResult getResult();
	void test();

	static void buildDataElement(char *pMessagePackage, const char *pMessage, int &nEvent, const char *pClientAccount, const char *pFriendAccount);
	static void buildFriendElement(char *pMessagePackage, const char *pDataName, const char *pDataAccount, const char *pDataStatue);
	static void buildElement(char *pMessagePackage, vector<string> &vStrData);

private:
	void processMetaData(string strSource);
	void processData(string strSource);
	//void buildDataElement(char *pClientMessage, int nEvent, string strClientAccount);
	int converterToInt(const char *pNum, int nLen);
private:
	ParserResult m_ParserResult;  //�˴δ����Ԫ���ݽ��

	static char m_cDataHead[3];
	static char m_cEvent[3];
	static char m_cMeta_DataInterval[2];
	static char m_cDataInterval[2];
	static char m_cDataElementTail[2];
	static char m_cDataFriendInterval[2];
};

