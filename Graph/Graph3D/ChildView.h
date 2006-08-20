// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__88A67612_E489_408A_8628_12AD9F88CBAB__INCLUDED_)
#define AFX_CHILDVIEW_H__88A67612_E489_408A_8628_12AD9F88CBAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Matrix.h"

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
protected:
	DECLARE_DYNCREATE(CChildView)

// Construction
public:
	CChildView();

// Attributes
public:
	CBitmap m_Bitmap;
	CDC     m_MemDC;
	
	Point3D m_Point[8];
	Point3D m_DrawPoint[8];
	Point3D m_COP;
	Point3D m_VRP;
	Point3D m_VPN;
	Point3D m_VUP;
	int     m_Umin;
	int     m_Umax;
	int     m_Vmin;
	int     m_Vmax;
	int     m_F;
	int     m_B;
	CMatrix m_Trans;
	CMatrix m_LineX;
	CMatrix m_LineXR;
	CMatrix m_LineY;
	CMatrix m_LineYR;
	CMatrix m_LineZ;
	CMatrix m_LineZR;
// Operations
public:
	void Clear(COLORREF clr);
	void DrawLines(COLORREF clr);
	void Draw();
	void CreateMatrix();
	void SetCOP(float x,float y,float z);
	void SetVPN(float x,float y,float z);
	void SetVUP(float x,float y,float z);
	void SetVRP(float x,float y,float z);
	void DrawOneLine(float x1,float y1,float x2,float y2,COLORREF clr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__88A67612_E489_408A_8628_12AD9F88CBAB__INCLUDED_)
