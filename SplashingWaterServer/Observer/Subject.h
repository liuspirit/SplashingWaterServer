/*
用于管理订阅的主题，如果主题修改，则可以通过通知发给订阅者
*/
#pragma once
#include"ObserverInterface.h"
#include <string>
#include <list>

class ObserverInterface;

struct SubjectData
{
	std::string strSource;
	std::string strDestination;

	int nInt;
	double dDouble;
	char pChar[2048];
	SubjectData()
	{
		strSource = "";
		strDestination = "";
		nInt = 0;
		dDouble = 0;
		memset(pChar, 0, sizeof(pChar));
	}
};

class Subject 
{
public:
	Subject();
	virtual ~Subject();
	
public:
	virtual void addObserver(ObserverInterface *pObserver);
	virtual void deleteObserver(ObserverInterface *pObserver);
	virtual void Notity();
protected:
	struct SubjectData m_SubjectData;
private:
	std::list<ObserverInterface *> m_ObserverList;
};

