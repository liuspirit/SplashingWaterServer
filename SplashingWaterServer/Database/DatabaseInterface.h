#pragma once
#include <vector>
#include <map>
using namespace std;

//select data
#define ROW_INTERVAL 0x40//0x06
#define FIELD_INTERVAL 0x10//0x05

class DatabaseInterface
{
public:
	virtual string selectData(const char *pTable, vector<string> fieldVec, const char *pCondition = NULL) = 0;
	virtual void insertData(const char *pTable, map<string, string> keyValueMap) = 0;
	virtual void deleteData(const char *pTable, map<string, string> keyValueMap) = 0;
	virtual void updateData(const char *pTable, map<string, string> keyValueMap, map<string, string> keyValueConMap) = 0;
	virtual void createTable(const char *pTable) = 0;
	virtual void deleteTable(const char *pTable) = 0;
	virtual void rawExecute(const char*pSQLScript) = 0;
};