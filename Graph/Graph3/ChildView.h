// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__11BE7F2C_6259_46D2_AEC9_29289F95CBA2__INCLUDED_)
#define AFX_CHILDVIEW_H__11BE7F2C_6259_46D2_AEC9_29289F95CBA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  CIRCLE     12
#define  LINE       4
#define  PI         3.1415926

/////////////////////////////////////////////////////////////////////////////
// CChildView window
typedef struct tag3DPoint{
	float x;
	float y;
	float z;
}Point3D;

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	CBitmap m_Bitmap;
	CDC     m_MemDC;
	int     m_Cx;
	int     m_Cy;

	Point3D  m_Point[CIRCLE][LINE];
	bool    m_Drawing;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	void Line(CDC* pdc,COLORREF clr,float x1,float y1,float x2,float y2);
	void Clear(CDC* pdc,COLORREF clrBack);
// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__11BE7F2C_6259_46D2_AEC9_29289F95CBA2__INCLUDED_)
