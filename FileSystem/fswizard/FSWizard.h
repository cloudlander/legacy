// FSWizard.h : main header file for the FSWIZARD application
//

#if !defined(AFX_FSWIZARD_H__B9C6954D_F39B_4C3D_92A7_3CBABEAF03CC__INCLUDED_)
#define AFX_FSWIZARD_H__B9C6954D_F39B_4C3D_92A7_3CBABEAF03CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AlphaSplashDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFSWizardApp:
// See FSWizard.cpp for the implementation of this class
//

class CFSWizardApp : public CWinApp
{
public:
	CFSWizardApp();
	CAlphaSplashDlg* m_pSplash;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFSWizardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFSWizardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSWIZARD_H__B9C6954D_F39B_4C3D_92A7_3CBABEAF03CC__INCLUDED_)
