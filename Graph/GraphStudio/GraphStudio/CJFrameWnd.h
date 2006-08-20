// CJFrameWnd.h : header file
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
 * $Revision: 10 $
 * $Archive: /CodeJockey/Include/CJFrameWnd.h $
 *
 * $History: CJFrameWnd.h $
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:17p
 * Updated in $/CodeJockey/Include
 * Cleaned up inline functions, and import/export macro so that class will
 * be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 6/11/99    Time: 8:25p
 * Updated in $/CodeJockey/Include
 * Slight modification made to popup menu code.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 6/09/99    Time: 8:33p
 * Updated in $/CodeJockey/Include
 * Ioannhs Stamatopoulos (Yiannhs) [ystamat@mail.datamedia.gr] - Extended
 * docking windows, removed ::GetSysColor(...) calls and WM_SYSCOLORCHANGE
 * message handler, this is now automatically handled by the base class.
 * Added redraw flags to the create method and modified paint and layout
 * handlers to reduce flicker and un-necessary command handling. Dragging
 * rect now reflects actual control bar docked size. Added "flat" look to
 * the control bars, the control bars can now use the classic DevStudio
 * look, or use flat gradient gripper, borders and frame buttons. Overall,
 * docking windows now handle dragging, docking and floating much better,
 * and behave more like DevStudio.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 5/02/99    Time: 2:18a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 5/02/99    Time: 2:13a
 * Updated in $/CodeJockey/Include
 * Re-wrote CCJMenu initialization routines with a more cleaner approach.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:47p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.  Added support for Brent Corkum's 
 * BCMenu (CCJMenu), which is a replacement for the CCoolMenu class.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 1/12/99    Time: 4:46p
 * Updated in $/CodeJockey/Include
 * Total re-write to add side-by-side docking support for CCJControlBar
 * class.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 4:46p
 * Created in $/CodeJockey/Include
 * Initial re-write and release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJFRAMEWND_H__
#define __CJFRAMEWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CJMenuBar.h"
#include "CJMenu.h"

// class forwards
class CCJSizeDockBar;

// CCJFrameWnd is a CFrameWnd derived class which is used to
// reposition toolbar layout for 3D effect, in addition to docking
// methods for CCJToolBar and CCJControlBar class
class  CCJFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CCJFrameWnd)

public:

	// Default constructor
	//
	CCJFrameWnd();

	// Virtual destructor
	//
	virtual ~CCJFrameWnd();

protected:

	static DWORD	dwDockBarMap[4][2];	// array used for docking masks
	CCJMenu			m_menu; 			// SDI frame menu.
	CCJMenuBar		m_wndMenuBar;		// menu bar

    // Yiannhs to parameterize and the dock bar border
	enum DockFlatBorders {
        brdrAllRaised       = 0x0000,
        brdrFlatTop         = 0x0001,
        brdrFlatBottom      = 0x0002,
        brdrFlatLeft        = 0x0004, 
        brdrFlatRight       = 0x0008,
        brdrFlatVertical    = 0x0003,
        brdrFlatHorizontal  = 0x000C,
        brdrFlatAll         = 0x000F
    };

protected:

	// this member function is called by the framework to associate
	// toolbar resources with the frame menu
	//
	HMENU NewMenu(
		// resource id for menu
		UINT nMenuID,
		// array of toolbar resource ids
		UINT* nToolbarIDs = NULL,
		// size of array.
		int nSize = 0);

	// this member function is called by the framework to associate
	// a toolbar resource with the frame menu
	//
	HMENU NewMenu(
		// resource id for menu
		UINT nMenuID,
		// toolbar resource
		UINT nToolbarID = NULL);

public:

	// this member function is called to initialize the CCJMenu
	//
	BOOL InitializeMenu(
		// resource id for menu
		UINT nMenuID,
		// array of toolbar resource ids
		UINT* nToolbarIDs = NULL,
		// size of array.
		int nSize = 0);

	// this member function is called to initialize the Menu
	//
	BOOL InitializeMenu(
		// resource id for menu
		UINT nMenuID,
		// toolbar resource
		UINT nToolbarID = NULL);

	// this member function is called to calculate the size of
	// a CCJControlBar
	//
	void RecalcAllExcept(CCJSizeDockBar *pBar);

	// this member function enables docking for standard 
	// toolbar and control bars
	//
	void EnableDocking(DWORD dwDockStyle);

	// this member function enables docking for CCJControl bars only
	//
	void EnableDockingSizeBar(
		// Specifies which sides of the frame window can serve as docking sites 
		// for control bars. It can be one or more of the following: 
		//
		// CBRS_ALIGN_TOP   Allows docking at the top of the client area.
		// CBRS_ALIGN_BOTTOM   Allows docking at the bottom of the client area.
		// CBRS_ALIGN_LEFT   Allows docking on the left side of the client area.
		// CBRS_ALIGN_RIGHT   Allows docking on the right side of the client area.
		// CBRS_ALIGN_ANY   Allows docking on any side of the client area. 
		DWORD dwDockStyle,
		// enum for flat border styles.
		DockFlatBorders FlatFlags = brdrAllRaised );

	// this member function will dock a CCJControlBar
	//
	void DockSizeBar(
		// toolbar to dock
		CControlBar* pBar,
		// dockbar object usually NULL
		CCJSizeDockBar* pDockBar=NULL,
		// initial size usually NULL
		LPRECT lpRect=NULL);

	// this memeber function will dock the control bar passed in
	// as the first argument, and dock the control bar that is passed in
	// as the second argument to the left of the first one.
	//
	void DockControlBarLeftOf(
		// control bar to dock to the right of
		CControlBar* Bar,
		// already docked control bar
		CControlBar* LeftOf);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJFrameWnd)
	public:
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CCJFrameWnd)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CJFRAMEWND_H__

