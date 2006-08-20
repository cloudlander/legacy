// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C30F3BDC_0FD8_4FA3_8328_0FCDFC5DE619__INCLUDED_)
#define AFX_MAINFRM_H__C30F3BDC_0FD8_4FA3_8328_0FCDFC5DE619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CJFrameWnd.h"
#include "CJMenuBar.h"
#include "CJToolBar.h"
#include "GfxSplitterWnd.h"
#include "ChildView.h"
#include "PrjListView.h"
#include "TeapotView.h"
#include "VasView.h"
class Bar
{
public:
	UINT id;
	BOOL visible;
};

class CMainFrame : public CCJFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)
	
// Attributes
public:
    CCJMenuBar          m_wndMenuBar;            // menu bar
    CStatusBar    m_wndStatusBar;
    CCJToolBar    m_wndToolBar;
	CCJToolBar	  m_TeapotBar;
	CCJToolBar	  m_VasBar;
	CCJToolBar	  m_ScanBar;
	CCJToolBar	  m_3DBar;
	CCJToolBar	  m_FloatBar;
    CGfxSplitterWnd     m_wndSplitter;
	BOOL		m_bShowList;
// Operations
public:
	void	ChangeView(int i);
	void DockControlBarLeftOf(CToolBar*Bar,CToolBar* LeftOf);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewPrj();
	afx_msg void OnUpdateViewPrj(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C30F3BDC_0FD8_4FA3_8328_0FCDFC5DE619__INCLUDED_)
