#if !defined(AFX_VASVIEW_H__EF3AA894_BFE1_48E9_AA43_3EC1FDA5A030__INCLUDED_)
#define AFX_VASVIEW_H__EF3AA894_BFE1_48E9_AA43_3EC1FDA5A030__INCLUDED_
#include "CDib.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VasView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVasView view

class CVasView : public CScrollView
{
protected:
	CVasView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVasView)

// Attributes
public:
	CDib	m_Dibfile;
	CRect	m_rect;
	CPoint	m_Org;
	CPoint	m_First;
	int		m_SizeX;
	int		m_SizeY;

	BOOL	m_bDraging;
	BOOL	m_DragRight;
	BOOL	m_DragLeft;
	BOOL	m_DragTop;
	BOOL	m_DragBottom;

	CBitmap m_Bitmap;
	CDC     m_MemDC;
	int Cx,Cy;

// Operations
public:
	void	LoadFile();
	void	VasBitmap();
	void	DrawBitmap();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVasView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVasView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CVasView)
	afx_msg void OnAntiBmp();
	afx_msg void OnOpenBmp();
	afx_msg void OnSaveBmp();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VASVIEW_H__EF3AA894_BFE1_48E9_AA43_3EC1FDA5A030__INCLUDED_)
