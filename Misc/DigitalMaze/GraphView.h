// GraphView.h : interface of the CGraphView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHVIEW_H__0515FEC5_7FA1_4B25_9764_9C1ABCEB02F6__INCLUDED_)
#define AFX_GRAPHVIEW_H__0515FEC5_7FA1_4B25_9764_9C1ABCEB02F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraphView : public CView
{
protected: // create from serialization only
	CGraphView();
	DECLARE_DYNCREATE(CGraphView)

// Attributes
public:
	CGraphDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnButton();
	bool InitGraph();
	virtual ~CGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void ChangeGraph();
	//{{AFX_MSG(CGraphView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool bStop;
	int nSecond;
	CStatic m_static;
	bool bAgain;
	CButton button;
	void PlayAgain();
	bool InitMemDC();
	void InitPlaceMatric();
	int CurrentPlace;
	int dir;  
	int IDB[9];
	CBitmap bitmap;
	CDC memdc[9];
	bool isSuccess();
	struct GraphDir{
		int dir[4];
	}place[9];
};

#ifndef _DEBUG  // debug version in GraphView.cpp
inline CGraphDoc* CGraphView::GetDocument()
   { return (CGraphDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEW_H__0515FEC5_7FA1_4B25_9764_9C1ABCEB02F6__INCLUDED_)
