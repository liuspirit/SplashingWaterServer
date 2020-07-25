//#include "../stdafx.h"
#include "Subject.h"
#include "../Log/LogManager.h"

Subject::Subject()
{

}
Subject::~Subject()
{

}

void Subject::addObserver(ObserverInterface *pObserver)
{
	m_ObserverList.push_back(pObserver);
}
void Subject::deleteObserver(ObserverInterface *pObserver)
{
	m_ObserverList.remove(pObserver);
}

void Subject::Notity()
{
	std::list<ObserverInterface *>::iterator it = m_ObserverList.begin();
	while (it != m_ObserverList.end())
	{
		(*it)->update(m_SubjectData);
		++it;
	}
}