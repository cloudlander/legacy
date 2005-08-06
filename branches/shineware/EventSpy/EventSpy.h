// EventSpy.h : main header file for the EVENTSPY DLL
//

#if !defined(AFX_EVENTSPY_H__831D8C99_1140_4C39_9E98_E01D40B7DE1B__INCLUDED_)
#define AFX_EVENTSPY_H__831D8C99_1140_4C39_9E98_E01D40B7DE1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEventSpyApp
// See EventSpy.cpp for the implementation of this class
//

class CEventSpyApp : public CWinApp
{
public:
	CEventSpyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventSpyApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEventSpyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTSPY_H__831D8C99_1140_4C39_9E98_E01D40B7DE1B__INCLUDED_)
