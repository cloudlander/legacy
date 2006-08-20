// Polydron.h : main header file for the POLYDRON application
//

#if !defined(AFX_POLYDRON_H__9939C605_B141_43A0_8359_BB2BCF50DD88__INCLUDED_)
#define AFX_POLYDRON_H__9939C605_B141_43A0_8359_BB2BCF50DD88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPolydronApp:
// See Polydron.cpp for the implementation of this class
//

class CPolydronApp : public CWinApp
{
public:
	CPolydronApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPolydronApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CPolydronApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYDRON_H__9939C605_B141_43A0_8359_BB2BCF50DD88__INCLUDED_)
