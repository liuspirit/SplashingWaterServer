#pragma once

/*
SmartPtr<int> nPtr = new int;
*nPtr = 2;
AfxMessageBox(Converter::toCString(*nPtr));
*/

template<typename T>
class SmartPtr
{
public:
	SmartPtr(T *p = NULL) :m_pSmartPtr(p)
	{
		if (p)
		{
			m_nCount = new size_t(1);
		}
		else
		{
			m_nCount = new size_t(0);
		}
	}
	SmartPtr(const SmartPtr& ptr)
	{
		if (this ! = &ptr)
		{
			this->m_pSmartPtr = ptr.m_pSmartPtr;
			this->m_nCount = ptr.m_nCount;
			(*this->m_nCount)++;
		}
	}

	~SmartPtr()
	{
		(*this->m_nCount)--;
		if (*this->m_nCount == 0)
		{
			delete this->m_pSmartPtr;
			delete this->m_nCount;
			this->m_pSmartPtr = NULL;
			this->m_nCount = NULL;
		}
	}

	SmartPtr& operator=(const SmartPtr& ptr)
	{
		if (this->m_pSmartPtr == ptr.m_pSmartPtr)
		{
			return *this;
		}
		if (this->m_pSmartPtr)
		{
			(*this->m_nCount)--;
			if (this->m_nCount == 0)
			{
				delete this->m_pSmartPtr;
				delete this->m_nCount;
			}
		}
		this->m_pSmartPtr = ptr.m_pSmartPtr;
		this->m_nCount = ptr.m_nCount;
		(*this->m_nCount)++;
		return *this;
	}

	T* operator->() const
	{
		return  this->m_pSmartPtr;
	}
	T& operator*() const
	{
		return *(this->m_pSmartPtr);
	}
	T& operator()() const
	{
		return *(this->m_pSmartPtr);
	}

	size_t use_count()
	{
		return *this->m_nCount;
	}

private:
	T *m_pSmartPtr;
	size_t* m_nCount;
};

