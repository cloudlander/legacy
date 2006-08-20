#if !defined(AFX_TEAPOTVIEW_H__1DB33E87_C6FF_473F_90E4_60E2F1DB8849__INCLUDED_)
#define AFX_TEAPOTVIEW_H__1DB33E87_C6FF_473F_90E4_60E2F1DB8849__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeapotView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTeapotView window

class CTeapotView : public CScrollView
{
	DECLARE_DYNCREATE(CTeapotView);
/*	typedef struct
{
    //short bfType;//Ϊ"BM"
    long bfSize;
    long dfReserved;
    long bfOffbits;
}CBitMapFileHead;
typedef struct
{
    long BitSize;
    long BitWidth;
    long BitHeight;
    short BitPlance;
    short BitCount;
    long BitCompression;
    long BitSizeImage;
    long BitXPels;
    long BitYPels;
    long BitColorUsed;
    long BitColorImportant;
}CBitMapHead;
*/
// Construction
public:
	CTeapotView();

// Attributes
public:
	CBitmap m_Bitmap;
	CDC     m_MemDC;
	static	int		Cx,Cy;
	Point3D HuTi[16];
	Point3D HuGai[7];
	Point3D HuZui[32];
	Point3D HuZui2[32];
	Point3D BaShou[32];
	Point3D BaShou2[32];

// Operations
public:
	void Bazel(CDC&,Point3D*,int,int,void(*p)(CDC&,Point3D&));
	void BazelEx(CDC&,Point3D*,int,int,void(*p)(CDC&,Point3D&));
	static void Line(CDC& dc,Point3D& x,Point3D& y);
	void Clear(COLORREF clr);
	void Draw();
	void SaveToBitmap(CString& szPath,CRect rect);
	void OnDraw(CDC* pDc);

	void OnInitialUpdate();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeapotView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTeapotView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTeapotView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSaveBmp();
	afx_msg void OnTeapotX();
	afx_msg void OnTeapotY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAPOTVIEW_H__1DB33E87_C6FF_473F_90E4_60E2F1DB8849__INCLUDED_)
