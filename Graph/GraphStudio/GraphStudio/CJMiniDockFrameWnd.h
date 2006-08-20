// CJMiniDockFrameWnd.h : header file
//
// Copyright © 1998-99 Kirk W. Stowell
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
 * $Revision: 6 $
 * $Archive: /CodeJockey/Include/CJMiniDockFrameWnd.h $
 *
 * $History: CJMiniDockFrameWnd.h $
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:13p
 * Updated in $/CodeJockey/Include
 * Cleaned up inline functions, and import/export macro so that class will
 * be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 6/14/99    Time: 10:46p
 * Updated in $/CodeJockey/Include
 * Cleaned up code to eliminate level 4 compilier warnings.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 4/14/99    Time: 10:40p
 * Updated in $/CodeJockey/Include
 * Daniel Harth [danielh@mail.matco.ro] - Added support for diagonal
 * sizing.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 10:39p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 1/16/99    Time: 10:39p
 * Created in $/CodeJockey/Include
 * Initial release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJMINIDOCKFRAMEWND_H__
#define __CJMINIDOCKFRAMEWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CCJMiniDockFrameWnd is a CMiniDockFrameWnd class which is used to
// handle docking for the CCJControlBar class.
class  CCJMiniDockFrameWnd : public CMiniDockFrameWnd
{
	DECLARE_DYNCREATE(CCJMiniDockFrameWnd)

public:

	// Default constructor
	//
	CCJMiniDockFrameWnd();

	// Virtual destructor
	//
	virtual ~CCJMiniDockFrameWnd();

protected:

	// make CDockBar a friend to this class for access to member data
	//
	friend class CDockBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJMiniDockFrameWnd)
	public:
	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CCJMiniDockFrameWnd)
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CJMINIDOCKFRAMEWND_H__

