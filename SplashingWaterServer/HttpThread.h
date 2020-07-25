#pragma once
#include <windows.h>

enum RunStatus{Run,Stop,Stoping};

class HttpThread
{
public:
	HttpThread();
	virtual ~HttpThread();

public:
	void setSleepTime(int nSleepTime);
	void start_Thread();
	void end_Thread();
	void Suspend_Thread();
	virtual void run();
	
	void Terminate_Thread();
	static DWORD __stdcall WThreadFunctionLinek(IN LPVOID Param);
	RunStatus getThreadStatus();

private:
	void RunInThread();

private:
	RunStatus m_RunStatus;
	//bool m_bRunning;
	int m_nSleepTime;
	//CWinThread* m_Thread;
	HANDLE m_Thread;			//线程句柄;
	DWORD m_nthreadID;		//线程ID;
};

