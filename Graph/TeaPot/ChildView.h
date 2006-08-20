// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__DC58C2FE_24D0_4F7A_9BA1_CC8ED2146F77__INCLUDED_)
#define AFX_CHILDVIEW_H__DC58C2FE_24D0_4F7A_9BA1_CC8ED2146F77__INCLUDED_
#include "Matrix.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*class CExMatrix:CMatrixT<float>
{
public:
	CExMatrix& CExMatrix::operator=(Point3D& point)
	{
	ASSERT(RowDim==1&&ColDim==4);
	matrixLine[0][0]=point.x;
	matrixLine[0][1]=point.y;
	matrixLine[0][2]=point.z;
	matrixLine[0][3]=1;
	return *this;
	}

};
*/

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
// Operations
	CBitmap m_Bitmap;
	CDC     m_MemDC;
public:
	void Bazel(CDC&,Point3D*,int,int,void(*p)(CDC&,Point3D&));
	void BazelEx(CDC&,Point3D*,int,int,void(*p)(CDC&,Point3D&));
	void Clear(COLORREF clr);
	void Draw();
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
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__DC58C2FE_24D0_4F7A_9BA1_CC8ED2146F77__INCLUDED_)
