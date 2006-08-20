// Graph3D.h : main header file for the GRAPH3D application
//

#if !defined(AFX_GRAPH3D_H__DE24FE05_0D3A_4957_BE52_D856F4534604__INCLUDED_)
#define AFX_GRAPH3D_H__DE24FE05_0D3A_4957_BE52_D856F4534604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraph3DApp:
// See Graph3D.cpp for the implementation of this class
//

class CGraph3DApp : public CWinApp
{
public:
	CGraph3DApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph3DApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CGraph3DApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH3D_H__DE24FE05_0D3A_4957_BE52_D856F4534604__INCLUDED_)
