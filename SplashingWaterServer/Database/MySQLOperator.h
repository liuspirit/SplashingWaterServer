#pragma once
#include "DatabaseInterface.h"
#include<mysql.h>  
#include<iostream>
#include <map>

#include<string>
using namespace std;

#pragma comment(lib, "libmysql.lib")


//error code
#define ACTION_SUCCESS 0
#define CONNECT_FAILED 1

class MySQLOperator : public DatabaseInterface
{
public:
	static MySQLOperator* getInstance(){
		if (m_pInstance == NULL)
		{
			m_pInstance = new MySQLOperator();
			m_pInstance->initialMySQL("lsy", "888888", "lsytest");
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

	//MySQLOperator(const char *pUser,const char *pPasswd,const char *pDatabase);

	virtual ~MySQLOperator();

public:
	virtual string selectData(const char *pTable, vector<string> fieldVec, const char *pCondition = NULL);

	virtual void insertData(const char *pTable, map<string, string> keyValueMap);
	virtual void deleteData(const char *pTable, map<string, string> keyValueMap);
	virtual void updateData(const char *pTable, map<string, string> keyValueMap, map<string, string> keyValueConMap);
	virtual void createTable(const char *pTable);
	virtual void deleteTable(const char *pTable);
	virtual void rawExecute(const char*pSQLScript);

	const char* getSQLErrorInfo();

	void testInstance();
protected:
	void initialMySQL(const char *pUser, const char *pPasswd, const char *pDatabase);
	void freeMySQL();

private:
	MySQLOperator();

	void transferMapToString(map<string, string> &keyValueMap, string &strKey, string &strValue);
	void transferMapToCondition(map<string, string> &keyValueMap, string &strCondition);
private:
	MYSQL m_mySQL;
	static MySQLOperator* m_pInstance;
};

