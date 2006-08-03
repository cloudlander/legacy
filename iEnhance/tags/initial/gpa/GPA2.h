// GPA2.h : main header file for the GPA2 DLL
//

#if !defined(AFX_GPA2_H__7C0FD0D9_9C89_48EB_B354_E0172EC7CDC3__INCLUDED_)
#define AFX_GPA2_H__7C0FD0D9_9C89_48EB_B354_E0172EC7CDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGPA2App
// See GPA2.cpp for the implementation of this class
//

class CGPA2App : public CWinApp
{
public:
	CGPA2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPA2App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGPA2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPA2_H__7C0FD0D9_9C89_48EB_B354_E0172EC7CDC3__INCLUDED_)
