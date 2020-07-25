#pragma once
#include "Subject.h"

class ObserverInterface
{
public:
	virtual void update(struct SubjectData subjectData) = 0;
};

