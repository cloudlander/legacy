// wmd5.h : main header file for the WMD5 application
//

#if !defined(AFX_WMD5_H__906AAE18_B2F8_4AA4_B03B_72A26E1B2B52__INCLUDED_)
#define AFX_WMD5_H__906AAE18_B2F8_4AA4_B03B_72A26E1B2B52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "workqueue.h"
/////////////////////////////////////////////////////////////////////////////
// CWmd5App:
// See wmd5.cpp for the implementation of this class
//

class CWmd5App : public CWinApp
{
public:
	CWmd5App();
//	CWorkQueue m_Queue;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWmd5App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWmd5App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

class MyCommandLineInfo : public CCommandLineInfo
{
public:
	MODE m_mode;

	MyCommandLineInfo();
	virtual ~MyCommandLineInfo();
	void ParseParam(const TCHAR* ,BOOL,BOOL);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMD5_H__906AAE18_B2F8_4AA4_B03B_72A26E1B2B52__INCLUDED_)
