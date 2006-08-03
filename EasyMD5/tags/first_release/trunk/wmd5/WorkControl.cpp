// WorkControl.cpp: implementation of the WorkControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wmd5.h"
#include "WorkControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WorkControl::WorkControl()
{	
	m_PageSize=64*1024;		// default page size
	m_pQueue=NULL;
	m_pThread=NULL;
	m_pUpdater=new CJobProgresser;
}

WorkControl::~WorkControl()
{
	if(IsWindow(m_pUpdater->m_hWnd))
		m_pUpdater->DestroyWindow();
	if(m_pUpdater)
		delete m_pUpdater;
}



JobUnit::JobUnit()
{
	status=READY;
	m_pOpenFile=NULL;
	fileName="";
	compareMd5="";
	InitializeCriticalSection(&stat_cs);
}

JobUnit::~JobUnit()
{
	if(m_pOpenFile)
		m_pOpenFile->Close();
	DeleteCriticalSection(&stat_cs);
}