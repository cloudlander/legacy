// SearchAvail.h : main header file for the SEARCHAVAIL application
//

#if !defined(AFX_SEARCHAVAIL_H__DD0D14AA_6D47_499C_B477_4A5C2FB812FA__INCLUDED_)
#define AFX_SEARCHAVAIL_H__DD0D14AA_6D47_499C_B477_4A5C2FB812FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSearchAvailApp:
// See SearchAvail.cpp for the implementation of this class
//

class CSearchAvailApp : public CWinApp
{
public:
	CSearchAvailApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchAvailApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSearchAvailApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHAVAIL_H__DD0D14AA_6D47_499C_B477_4A5C2FB812FA__INCLUDED_)
