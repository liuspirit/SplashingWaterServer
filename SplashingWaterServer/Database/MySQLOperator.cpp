//#include "../stdafx.h"
#include "MySQLOperator.h"
#include "../Common/StrTool.h"
MySQLOperator* MySQLOperator::m_pInstance = NULL;

MySQLOperator::MySQLOperator()
{

}
/*
MySQLOperator::MySQLOperator(const char *pUser, const char *pPasswd, const char *pDatabase)
{
	initialMySQL(pUser, pPasswd, pDatabase);
}
*/

MySQLOperator::~MySQLOperator()
{
	freeMySQL();
}

void MySQLOperator::initialMySQL(const char *pUser, const char *pPasswd, const char *pDatabase)
{
	mysql_init(&m_mySQL);
	if (!mysql_real_connect(&m_mySQL, "localhost", pUser, pPasswd, pDatabase, 3306, NULL, 0))
	{

		throw exception("Failed to connect database! ");
	}
}

void MySQLOperator::freeMySQL()
{
	mysql_close(&m_mySQL);
}

void MySQLOperator::insertData(const char *pTable, map<string, string> keyValueMap)
{
	string strKey, strValue;
	char pSQL[2048];

	transferMapToString(keyValueMap, strKey, strValue);
	sprintf(pSQL, "Insert into %s(%s) values(%s);", pTable, strKey.c_str(), strValue.c_str());
	if (mysql_query(&m_mySQL, pSQL) != 0)
	{
		throw exception("Failed to insert Data");
	}
}

void MySQLOperator::deleteData(const char *pTable, map<string, string> keyValueMap)
{
	string strCondition;
	char pSQL[2048];

	transferMapToCondition(keyValueMap, strCondition);
	sprintf(pSQL, "delete from %s where %s;", pTable, strCondition.c_str());
	if (mysql_query(&m_mySQL, pSQL) != 0)
	{
		throw exception("Failed to delete Data");
	}

}

void MySQLOperator::updateData(const char *pTable, map<string, string> keyValueMap, map<string, string> keyValueConMap)
{
	string strCondition;
	string strkeyValue;
	char pSQL[2048];
	transferMapToCondition(keyValueMap, strkeyValue);
	transferMapToCondition(keyValueConMap, strCondition);
	sprintf(pSQL, "update %s set %s where %s;", pTable, strkeyValue.c_str(), strCondition.c_str());
	if (mysql_query(&m_mySQL, pSQL) != 0)
	{
		throw exception("Failed to update Data");
	}
}

void MySQLOperator::createTable(const char *pTable)
{
	//char pSQL[2048];
	//sprintf(pSQL, "create database %s;", pTable);
	//if (mysql_query(&m_mySQL, pSQL) != 0)
	//{
	//	throw exception("Failed to create Table");
	//}
}

void MySQLOperator::rawExecute(const char*pSQLScript)
{
	if (mysql_query(&m_mySQL, pSQLScript) != 0)
	{
		throw exception("Failed to execute script");
	}
}

void MySQLOperator::deleteTable(const char *pTable)
{
	char pSQL[2048];
	sprintf(pSQL, "drop table  %s;", pTable);
	if (mysql_query(&m_mySQL, pSQL) != 0)
	{
		throw exception("Failed to delete Table");
	}
}

string MySQLOperator::selectData(const char *pTable, vector<string> fieldVec, const char *pCondition)
{
	char pCol[2048];
	memset(pCol, 0,sizeof(pCol));
	for (int i = 0; i < fieldVec.size(); i++)
	{
		if (i != 0)
		{
			strcat(pCol, ",");
		}
		strcat(pCol, fieldVec.at(i).c_str());
	}

	char pSQL[2048];
	if (pCondition != NULL)
	{
		sprintf(pSQL, "select %s from %s where %s;", pCol, pTable, pCondition);
	}
	else
	{
		sprintf(pSQL, "select %s from %s;", pCol, pTable);
	}
	//cout << pSQL << endl;
	mysql_query(&m_mySQL, "set names gbk");
	if (mysql_query(&m_mySQL, pSQL) != 0)
	{
		throw exception("Failed to search data!");
	}
	
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集
	if (!(res = mysql_store_result(&m_mySQL)))
	{
		throw exception("Failed to get search data");
	}
	
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
	int nFirst = 0;


	string strResult = "";
	//char cRowInterval = ROW_INTERVAL; //行隔开

	char cRowInterval = FIELD_INTERVAL;
	char cFieldInterval = FIELD_INTERVAL; //字段隔开
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行
	{
		
		if (nFirst!=0)
		{
			strResult = strResult + cFieldInterval;
		}
		int nNum = 0;
		
		while (nNum<fieldVec.size())
		{
			if (nNum != 0)
			{
				strResult = strResult + cFieldInterval;
			}
			strResult = strResult + column[nNum];
			nNum = nNum + 1;
		}
		nFirst++;
	}
	
	mysql_free_result(res);
	return strResult;
}

const char* MySQLOperator::getSQLErrorInfo()
{
	return mysql_error(&m_mySQL);
}

void MySQLOperator::transferMapToString(map<string, string> &keyValueMap, string &strKey, string &strValue)
{
	map<string, string>::iterator ite = keyValueMap.begin();
	strKey = "";
	strValue = "";
	while (ite != keyValueMap.end())
	{
		if (ite != keyValueMap.begin())
		{
			strKey = strKey + ",";
			strValue = strValue + ",";
		}
		strKey = strKey + ite->first;
		strValue = strValue + ite->second;
		ite++;
	}
}
void MySQLOperator::transferMapToCondition(map<string, string> &keyValueMap, string &strCondition)
{
	map<string, string>::iterator ite = keyValueMap.begin();
	strCondition = "";
	while (ite != keyValueMap.end())
	{
		if (ite != keyValueMap.begin())
		{
			strCondition = strCondition + " and ";
		}
		strCondition = strCondition + ite->first + "=" + ite->second;
		ite++;
	}
}
void MySQLOperator::testInstance()
{
	/*
	DatabaseInterface *pDatabase = new MySQLOperator("lsy", "888888", "lsytest");
	vector<string> fieldVec;
	fieldVec.push_back("id");
	fieldVec.push_back("name");
	fieldVec.push_back("sex");
	fieldVec.push_back("degree");
	try
	{
		string strResult = pDatabase->selectData("student", fieldVec);
		cout << strResult << endl;
		map<string, string> insertMap;
		insertMap.insert(pair<string, string>("name", "'Mickic'"));
		insertMap.insert(pair<string, string>("sex", "0"));
		insertMap.insert(pair<string, string>("degree", "34.2"));
		pDatabase->insertData("student", insertMap);

		strResult = pDatabase->selectData("student", fieldVec);
		cout << strResult << endl;


		insertMap.clear();
		insertMap.insert(pair<string, string>("name", "'Mickic'"));
		pDatabase->deleteData("student", insertMap);

		strResult = pDatabase->selectData("student", fieldVec);
		cout << strResult << endl;
	}
	catch (const exception &e)
	{
		cout << "error:" << e.what() << endl;
	}

	delete pDatabase;
	pDatabase = NULL;
	
	
	
	*/

}