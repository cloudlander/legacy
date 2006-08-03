// ctxmenu.h : main header file for the CTXMENU DLL
//

#if !defined(AFX_CTXMENU_H__01A98056_2FFC_4CD3_8AE9_6C939465E98F__INCLUDED_)
#define AFX_CTXMENU_H__01A98056_2FFC_4CD3_8AE9_6C939465E98F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCtxmenuApp
// See ctxmenu.cpp for the implementation of this class
//

class CCtxmenuApp : public CWinApp
{
public:
	CCtxmenuApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtxmenuApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCtxmenuApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern UINT      g_cRefThisDll;


// {6CB24F4F-79C5-4c6a-905C-E58FA31132CA}
DEFINE_GUID(CLSID_EASYMD5, 
			0x6cb24f4f, 0x79c5, 0x4c6a, 0x90, 0x5c, 0xe5, 0x8f, 0xa3, 0x11, 0x32, 0xca);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTXMENU_H__01A98056_2FFC_4CD3_8AE9_6C939465E98F__INCLUDED_)
