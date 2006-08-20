#if !defined(AFX_FLOATVIEW_H__EA28DF97_C065_4685_9FBD_1C3A83512F84__INCLUDED_)
#define AFX_FLOATVIEW_H__EA28DF97_C065_4685_9FBD_1C3A83512F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatView view

class CFloatView : public CScrollView
{
protected:
	CFloatView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFloatView)

// Attributes
public:
	CBitmap m_Bitmap;
	CDC     m_MemDC;
	int Cx,Cy;
// Operations
public:
	void FloatLevel(CDC& dc,double Xmin,double Xmax,double Zmin,double Zmax,double(*f)(double,double),void(*p)(double&,double&,double));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFloatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFloatView)
	afx_msg void OnProperty();
	afx_msg void OnSaveBmp();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATVIEW_H__EA28DF97_C065_4685_9FBD_1C3A83512F84__INCLUDED_)
