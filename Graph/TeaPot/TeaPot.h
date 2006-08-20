// TeaPot.h : main header file for the TEAPOT application
//

#if !defined(AFX_TEAPOT_H__72E33270_4F74_4164_8D42_BECFC64AD3C3__INCLUDED_)
#define AFX_TEAPOT_H__72E33270_4F74_4164_8D42_BECFC64AD3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTeaPotApp:
// See TeaPot.cpp for the implementation of this class
//

class CTeaPotApp : public CWinApp
{
public:
	CTeaPotApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeaPotApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CTeaPotApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAPOT_H__72E33270_4F74_4164_8D42_BECFC64AD3C3__INCLUDED_)
