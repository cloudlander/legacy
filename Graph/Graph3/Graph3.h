// Graph3.h : main header file for the GRAPH3 application
//

#if !defined(AFX_GRAPH3_H__32C07591_E560_41EB_AF38_D49FF49A75F1__INCLUDED_)
#define AFX_GRAPH3_H__32C07591_E560_41EB_AF38_D49FF49A75F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraph3App:
// See Graph3.cpp for the implementation of this class
//

class CGraph3App : public CWinApp
{
public:
	CGraph3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CGraph3App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH3_H__32C07591_E560_41EB_AF38_D49FF49A75F1__INCLUDED_)
