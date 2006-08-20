// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__91F48060_820B_4668_86A6_6E89F188D5AA__INCLUDED_)
#define AFX_CHILDVIEW_H__91F48060_820B_4668_86A6_6E89F188D5AA__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CHtmlView
{
	DECLARE_DYNCREATE(CChildView)
// Construction
public:
	CChildView();

// Attributes
public:
// Operations
public:
	
// Overrides
	public:
	virtual void OnInitialUpdate();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__91F48060_820B_4668_86A6_6E89F188D5AA__INCLUDED_)
