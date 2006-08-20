#if !defined(_SCAN_LING_H)
#define _SCAN_LING_H

#include "CaptureBitmap.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanLineView.h : header file
//

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
// CScanLineView view

class CScanLineView : public CScrollView
{
protected:
	CScanLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScanLineView)

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
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
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
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanLineView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScanLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScanLineView)
	afx_msg void OnSaveBmp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANLINEVIEW_H__5D0EA1DA_AC8F_4A48_BEF1_1EE47BDDCAD3__INCLUDED_)
