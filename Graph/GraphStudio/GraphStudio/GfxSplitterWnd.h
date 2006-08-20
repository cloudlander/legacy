// Copyright (c) Iuri Apollonio 1998
// Use & modify as you want & need, and leave those 3 lines.
// http://www.codeguru.com


// GfxSplitterWnd.h: interface for the CGfxSplitterWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GFXSPLITTERWND_H__
#define __GFXSPLITTERWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class  CGfxSplitterWnd : public CSplitterWnd  
{
public:
	int m_upBorder;
	bool bWhiteLine;
	CGfxSplitterWnd();
	virtual ~CGfxSplitterWnd();
	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	void OnInvertTracker(const CRect & rect);

	void RecalcLayout();
protected:
	void GetInsideRect(CRect& rect) const;
	// Generated message map functions
	//{{AFX_MSG(CGfxSplitterWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __GFXSPLITTERWND_H__
