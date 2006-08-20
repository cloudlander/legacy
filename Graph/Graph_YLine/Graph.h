// Graph.h : main header file for the GRAPH application
//

#if !defined(AFX_GRAPH_H__233450BF_0582_4041_BB68_D92A134B23A5__INCLUDED_)
#define AFX_GRAPH_H__233450BF_0582_4041_BB68_D92A134B23A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraphApp:
// See Graph.cpp for the implementation of this class
//

class CGraphApp : public CWinApp
{
public:
    CGraphApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation
protected:
    HMENU m_hMDIMenu;
    HACCEL m_hMDIAccel;

public:
    //{{AFX_MSG(CGraphApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileNew();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__233450BF_0582_4041_BB68_D92A134B23A5__INCLUDED_)
