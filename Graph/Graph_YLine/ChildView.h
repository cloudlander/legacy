// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__173D491E_7B77_49E0_8262_263349A0F0F9__INCLUDED_)
#define AFX_CHILDVIEW_H__173D491E_7B77_49E0_8262_263349A0F0F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <list>

using namespace std;

class Line
{
public:
    float x;
    float dx;
    int dy;

public:
    Line(int xx,float ddx,int ddy){
        x=xx;  dx=ddx; dy=ddy;
    }
    Line(){};
    bool operator<(Line& ll){
        if(x < ll.x)
            return true;
        else
            return false;
    }
/*
	bool operator<(LineList& ll){
		if(*this > ll || *this == ll)
			return false;
		else
			return true;
		
	}
*/
  Line& operator=(const Line& ll)
    {
        x=ll.x;
        dx=ll.dx;
        dy=ll.dy;
        return *(this);
    }
};

typedef list<Line> LineList;

class YLineList
{
public:
    LineList yylist;
    int   y;

    YLineList(LineList& yy,int dy)
    {
        yylist = yy;
        y=dy;
    }

};



typedef list<YLineList>  YBucket;


/////////////////////////////////////////////////////////////////////////////
// CChildView window
class CChildView : public CWnd
{
// Construction
public:
    CChildView();

// Attributes
public:
	CPoint m_Poly[255];
	CPoint m_folPoint;
	bool   m_bDrawing;
	int    m_Count;
	bool   m_bCanClose;
	bool   m_bCapture;

	CBitmap m_Bitmap;
	CDC     m_MemDC;

	int     m_Cx;
	int     m_Cy;

	COLORREF m_BackClr;
	COLORREF m_FillClr;

// Operations
public:

    void FillDraw(CPoint a[] ,int num,CDC* dc,COLORREF cl);

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
	afx_msg void OnStart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStop();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBackclr();
	afx_msg void OnFrontclr();
	afx_msg void OnUpdateBackclr(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__173D491E_7B77_49E0_8262_263349A0F0F9__INCLUDED_)
