#if !defined(AFX_PRJLISTVIEW_H__4615B51E_585A_4FFA_873C_1F1E1FEDA4E2__INCLUDED_)
#define AFX_PRJLISTVIEW_H__4615B51E_585A_4FFA_873C_1F1E1FEDA4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrjListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrjListView window

class CPrjListView : public CWnd
{
	DECLARE_DYNCREATE(CPrjListView)
// Construction
public:
	CPrjListView();

// Attributes
public:
	CCJTreeCtrl		m_TreeCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrjListView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrjListView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrjListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRJLISTVIEW_H__4615B51E_585A_4FFA_873C_1F1E1FEDA4E2__INCLUDED_)
