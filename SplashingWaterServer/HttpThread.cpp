#include "stdafx.h"
#include "HttpThread.h"


HttpThread::HttpThread()
{
	m_RunStatus = Stop;
	m_nSleepTime = 20;
	m_Thread = CreateThread(NULL, 0, WThreadFunctionLinek, this, CREATE_SUSPENDED, &m_nthreadID);
	Log_Info("HttpThread::HttpThread()------create new thread :%d", m_nthreadID);
}


HttpThread::~HttpThread()
{	
	end_Thread();
}

void HttpThread::setSleepTime(int nSleepTime)
{
	m_nSleepTime = nSleepTime;
}

DWORD __stdcall HttpThread::WThreadFunctionLinek(IN LPVOID Param)
{
	HttpThread *pThread = (HttpThread *)Param;
	pThread->RunInThread();
	return 0;
}

void HttpThread::RunInThread()
{
	while (m_RunStatus == Run)
	{
		run();
		//Sleep(m_nSleepTime);
	}
}
void HttpThread::run()
{

}

void HttpThread::start_Thread()
{
	if (m_RunStatus == Run)
	{
		return;
	}
	m_RunStatus = Run;
	ResumeThread(m_Thread);//»½ÐÑÏß³Ì
	Log_Info("HttpThread::start_Thread()------start thread");
}

void HttpThread::end_Thread()
{
	if (m_RunStatus == Stop || m_RunStatus == Stoping)
	{
		return;
	}
	m_RunStatus = Stoping;
	Log_Info("HttpThread::end_Thread()------wait to end thread...");
	//WaitForSingleObject(m_Thread, INFINITE);
	CloseHandle(m_Thread);
	m_RunStatus = Stop;
	Log_Info("HttpThread::end_Thread()------delete thread :%d", m_nthreadID);
}

void HttpThread::Terminate_Thread()
{
	if (m_RunStatus == Stop)
	{
		return;
	}
	TerminateThread(m_Thread, 0);
	CloseHandle(m_Thread);
	m_RunStatus = Stop;
	Log_Info("HttpThread::Terminate_Thread()------delete thread :%d", m_nthreadID);
}

void HttpThread::Suspend_Thread()
{
	SuspendThread(m_Thread);
}

RunStatus HttpThread::getThreadStatus()
{
	return m_RunStatus;
}