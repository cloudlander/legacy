// installer.h : main header file for the INSTALLER application
//

#if !defined(AFX_INSTALLER_H__5D0EACD3_A3DE_40FB_B2B3_FC5B07E24D8F__INCLUDED_)
#define AFX_INSTALLER_H__5D0EACD3_A3DE_40FB_B2B3_FC5B07E24D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInstallerApp:
// See installer.cpp for the implementation of this class
//

class CInstallerApp : public CWinApp
{
public:
	CInstallerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInstallerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLER_H__5D0EACD3_A3DE_40FB_B2B3_FC5B07E24D8F__INCLUDED_)
