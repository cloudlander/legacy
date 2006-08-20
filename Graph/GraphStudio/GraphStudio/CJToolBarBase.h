// CJToolBarBase.h : header file
//
// The majority of this source was copied from MFC, Visual C++ 6, to 
// extend functionality to Visual C++ 5 users, with some ideas taken
// from Luis Barreira's article 'Classes for new IE4 controls'
// http://www.codeguru.com/controls/ie4_controls_classes.shtml
//
// Copyright © 1998-1999 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This source code may be used in compiled form in any way you desire.
// Source file(s) may be redistributed unmodified by any means PROVIDING 
// they ARE NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the authors name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright © 1998-99 Kirk Stowell" must be included in the startup 
// banner, "About" box or printed documentation. This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 7/25/99 12:42a $
 * $Revision: 5 $
 * $Archive: /CodeJockey/Include/CJToolBarBase.h $
 *
 * $History: CJToolBarBase.h $
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:12a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:10p
 * Updated in $/CodeJockey/Include
 * Cleaned up inline functions, and import/export macro so that class will
 * be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 7/14/99    Time: 10:29p
 * Created in $/CodeJockey/Include
 * Copied from MFC v6 and techno preview for v5. Added to extend
 * functionality to Visual C++ 5.0 users.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CCJTOOLBARBASE_H__
#define __CCJTOOLBARBASE_H__

#ifdef _VC_VERSION_5
typedef struct tagAFX_OLDTOOLINFO
{
	UINT cbSize;
	UINT uFlags;
	HWND hwnd;
	UINT uId;
	RECT rect;
	HINSTANCE hinst;
	LPTSTR lpszText;
} AFX_OLDTOOLINFO;

#define CBRS_GRIPPER					0x00400000L
#endif // _VC_VERSION_5

// CCJToolBarBase is a CControlBar derived class whis is used 
// by the CCJToolBar class.
class  CCJToolBarBase : public CControlBar
{
	DECLARE_DYNAMIC(CCJToolBarBase)
		// Construction
protected:
	CCJToolBarBase();
	
// Attributes
	// getting and setting border space
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
	CRect GetBorders() const;

// Implementation
public:
	bool	m_bInReBar;	// true if rebar is parent.
	bool	m_bExStyle;	// true if created with the CreateEx style.
	
	virtual ~CCJToolBarBase();
	virtual void DoPaint(CDC* pDC);
	void DrawGripper(CDC* pDC, const CRect& rect);
	
	// implementation helpers
	virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const; // adjusts borders etc
	void EraseNonClient();
	void DrawBorders(CDC* pDC, CRect& rect);
	
	//{{AFX_MSG(CCJToolBarBase)
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// inline functions
/////////////////////////////////////////////////////////////////////////////

inline void CCJToolBarBase::SetBorders(LPCRECT lpRect)
	{ SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); }

#endif //__CCJTOOLBARBASE_H__

/////////////////////////////////////////////////////////////////////////////

