// WorkControl.h: interface for the WorkControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKCONTROL_H__4D9F07FE_902F_4089_88C3_555B024A89FC__INCLUDED_)
#define AFX_WORKCONTROL_H__4D9F07FE_902F_4089_88C3_555B024A89FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "jobprogresser.h"
class CWorkQueue;

typedef enum {FINISH,READY,RUNNING,DONE,SKIPPED,CANCELED,FAILURE,MATCH,NOTMATCH} STATUS;
/* thread control block */
struct WorkControl  
{
public:
	WorkControl();
	virtual ~WorkControl();
	
	DWORD m_PageSize;
	CWorkQueue* m_pQueue;
	CWinThread* m_pThread;
	CJobProgresser* m_pUpdater;
};

/* description of a job */
struct JobUnit
{
public:
	JobUnit();
	virtual ~JobUnit();
	CFile* m_pOpenFile;
	CString fileName;
	CString filePath;
	CString digest;
	CString compareMd5;
	volatile STATUS status;	
	LONGLONG filePages;

	/*  main thread and worker will touch status */
	inline void SetStatus(STATUS s)
	{
		ASSERT(s<=NOTMATCH && s>=RUNNING);
		EnterCriticalSection(&stat_cs);
		status=s;
		LeaveCriticalSection(&stat_cs);
	}
	inline STATUS GetStatus()
	{
		STATUS s;
		EnterCriticalSection(&stat_cs);
		s=status;
		LeaveCriticalSection(&stat_cs);
		return s;
	}

private:
	CRITICAL_SECTION stat_cs;
};
#endif // !defined(AFX_WORKCONTROL_H__4D9F07FE_902F_4089_88C3_555B024A89FC__INCLUDED_)
