// WorkQueue.h: interface for the CWorkQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKQUEUE_H__0325D5E2_BB74_4253_8C63_672984871B97__INCLUDED_)
#define AFX_WORKQUEUE_H__0325D5E2_BB74_4253_8C63_672984871B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "jobprogresser.h"
#include "workcontrol.h"
#include <vector>
using std::vector;
/********************************************************************
	created:	2005/03/03
	created:	3:3:2005   21:54
	filename: 	h:\my_projects\easymd5\wmd5\workqueue.h
	file path:	h:\my_projects\easymd5\wmd5
	file base:	workqueue
	file ext:	h
	author:		Ronny(zhangxiaomin@gmail.com)
	
	purpose:	this class defines a queue which handles all MD5SUM job
*********************************************************************/
typedef enum{GENMD5,CHECKMD5}MODE;


class CWmd5Dlg;
class MyCommandLineInfo;
class CWorkQueue  
{
public:
	void ResumeWork();
	void SuspendWork();
	void ShowResult();
	void Wait();
#ifdef SHOW_TIME_LEFT
	void TimeLeft(double);
#endif

	void DoSaveResult();
	void Update();
	void Run();
	JobUnit* GetNextJob(int& index);
	CWorkQueue();
	virtual ~CWorkQueue();
	
	bool Initialize(MyCommandLineInfo* cmdline,CWmd5Dlg* dlg);

	inline STATUS GetStatus(){
		STATUS s;
		EnterCriticalSection(&stat_cs);
		s=status;
		LeaveCriticalSection(&stat_cs);
		return s;
	}
	inline void SetStatus(STATUS s){
		ASSERT(s==CANCELED || s==DONE);
		if(DONE==status && CANCELED==s ||	// ignore
			CANCELED==status && DONE==s)
			return;
		EnterCriticalSection(&stat_cs);
		status=s;
		LeaveCriticalSection(&stat_cs);
		if(s==DONE) return;
		for(int i=0;i<m_vecJobs.size();i++)
		{
			if(m_vecJobs[i]->status==READY || m_vecJobs[i]->status==RUNNING)
				m_vecJobs[i]->status=CANCELED;
		}
		m_nFinishJobs=m_vecJobs.size();
	}
	inline void IncFinishJobs(int i=1)
	{
		EnterCriticalSection(&stat_cs);
		m_nFinishJobs+=i;
		LeaveCriticalSection(&stat_cs);
	}
	
		
	vector<WorkControl*> m_vecThreads;
	vector<JobUnit*> m_vecJobs;
	volatile STATUS status;
	MODE mode;
	CQueueProgresser* m_pUpdater;
	CWmd5Dlg* m_pDlg;
	CString m_szRootDir;

#ifdef SHOW_TIME_LEFT
	CStatic* m_pTime;
	unsigned int m_nDonePages;
	inline void IncDonePages(int i)
	{
		EnterCriticalSection(&stat_cs);
		m_nDonePages+=i;
		LeaveCriticalSection(&stat_cs);
	}
#endif

	int m_nFinishJobs;
	unsigned int m_nTotPages;
protected:
	DWORD m_SysAllocPages;
	BOOL FetchCompareJobs(CStdioFile&);
	BOOL FetchMd5Jobs(CStdioFile&);
	BOOL FetchJobs(CString& );
private:
	int m_nNextJob;
	CRITICAL_SECTION sche_cs;
	CRITICAL_SECTION stat_cs;
};

#endif // !defined(AFX_WORKQUEUE_H__0325D5E2_BB74_4253_8C63_672984871B97__INCLUDED_)
