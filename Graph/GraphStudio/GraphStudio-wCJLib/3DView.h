#if !defined(AFX_3DVIEW_H__ADB225AC_EA0D_475E_B7BF_4C57E3DB498A__INCLUDED_)
#define AFX_3DVIEW_H__ADB225AC_EA0D_475E_B7BF_4C57E3DB498A__INCLUDED_
#include "3DSetup.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DView view

class C3DView : public CScrollView
{
protected:
	C3DView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(C3DView)

// Attributes
public:
	CBitmap m_Bitmap;
	CDC     m_MemDC;
	
	int		Cx,Cy;
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

	C3DSetup m_3Ddlg;
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
	//{{AFX_VIRTUAL(C3DView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~C3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(C3DView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSaveBmp();
	afx_msg void OnShowSetup();
	afx_msg void OnUpdateShowSetup(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEW_H__ADB225AC_EA0D_475E_B7BF_4C57E3DB498A__INCLUDED_)
