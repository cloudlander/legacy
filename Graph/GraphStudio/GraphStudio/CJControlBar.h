// CJControlBar.h : header file
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
 * $Revision: 17 $
 * $Archive: /CodeJockey/Include/CJControlBar.h $
 *
 * $History: CJControlBar.h $
 * 
 * *****************  Version 17  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 16  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 15  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:18p
 * Updated in $/CodeJockey/Include
 * Added derived button class to handle control bar caption button
 * updates. Cleaned up inline functions, and import/export macro so that
 * class will be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 14  *****************
 * User: Kirk Stowell Date: 6/11/99    Time: 8:24p
 * Updated in $/CodeJockey/Include
 * Slight modification made to popup menu code.
 * 
 * *****************  Version 13  *****************
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
 * *****************  Version 12  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 9:52p
 * Updated in $/CodeJockey/Include
 * Changed gripper and button macros to enums. Renamed popup member
 * variables to begin with popup, this will be consistent throughout all
 * classes. Added m_nPos member variable which can be set to the index of
 * the popup menu in the menu resource. Removed windows message handler
 * ON_WM_CONTEXTMENU(), and added handlers ON_WM_RBUTTONDOWN(),
 * ON_WM_MEASUREITEM(), ON_WM_MENUCHAR() and ON_WM_INITMENUPOPUP() to
 * support the CCJMenu popup. Added WS_CLIPSIBLINGS to control bar style.
 * Changed OnEraseBkgnd(CDC* pDC) to only draw client area when a child is
 * not present.
 * 
 * *****************  Version 11  *****************
 * User: Kirk Stowell Date: 4/14/99    Time: 12:54p
 * Updated in $/CodeJockey/Include
 * Modified context menu to use Brent Corkum's BCMenu (CCJMenu).
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 12:52p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 3/13/99    Time: 12:54p
 * Updated in $/CodeJockey/Include
 * Patrick Bergeron [patb@softimage.com] fixed the following bugs:
 * 
 * I had a dialog nested in a CJControlBar. Unfortunately when I tried to
 * dock it to the left or right, the bottom part of my dialog would be
 * clipped off. 
 * 
 * Fix: When in docked mode, ::CalcDynamicLayout needed to return the
 * desired size, while taking into account the size of the gripper bar (if
 * any). Simply added a simple "+GetMinExt()" Now my dialog's bottom
 * doesn't get clipped. Woo-ha.
 * 
 * When nesting a CDialog based window inside a CJControlBar, the 3D rect
 * drawn by ::OnEraseBkgnd would be overwritten by the top and left dialog
 * borders. The problem was caused by the fact that in the
 * ::OnWindowPosChanged() method, the rectangle which is used to move the
 * window contains the border. A simple call to rc.DeflateRect(1,1) solved
 * this problem.
 * 
 * Added a call to UpdateWindow() in ::OnEraseBkgnd(). This helps with the
 * flickering a bit, but I Am not sure this is the correct way to go.
 * Incorporate at your own risks.
 * 
 * Added 2 ASSERT()s in ::OnMouseMove(). This was not to fix a bug, but at
 * one point I was seeing strange things that lead me to add these
 * asserts. They don't do anything bad, so why remove them?
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 3/10/99    Time: 12:50p
 * Updated in $/CodeJockey/Include
 * Fixed memory leak with button tooltips.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 3/10/99    Time: 12:48p
 * Updated in $/CodeJockey/Include
 * Added AfxRegisterWndClass() to create method to handle double clicks.
 * Thanks to Takehiko Mizoguti [mizoguti@m2.sys.to.casio.co.jp] for some
 * thoughts on this.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 2/28/99    Time: 12:47p
 * Updated in $/CodeJockey/Include
 * Calls default wnd proc if no context menu is defined Gonzalo Pereyra
 * [persys@adinet.com.uy]
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/31/99    Time: 12:48p
 * Updated in $/CodeJockey/Include
 * Removed extra line (typo) from OnLButtonUp(). Thanks to Ioannis
 * Stamatopoulos (ystamat@mail.datamedia.gr) for pointing this out.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/12/99    Time: 12:46p
 * Updated in $/CodeJockey/Include
 * Total class re-write, added multiple/side-by-side controlbar docking.
 * No longer uses CSizingControlBar base class.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/22/98   Time: 12:47p
 * Updated in $/CodeJockey/Include
 * Modified set cursor to display normal size cursor when static linked.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/20/98   Time: 12:45p
 * Updated in $/CodeJockey/Include
 * Fixed problem with gripper and buttons disappearing when docking
 * toggled. Overloaded IsFloating() method from base class.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 12:44p
 * Created in $/CodeJockey/Include
 * Initial re-write and release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJCONTROLBAR_H__
#define __CJCONTROLBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CJFlatButton.h"
#include "CJMenu.h"        // CCJMenu class declaration

// CCJCBarButton is a CCJFlatButton derived class which is used by
// the CCJControlBar class.
class  CCJCBarButton : public CCJFlatButton
{
protected:
	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem);
};

// CCJControlBar is a CControlBar derived class which is simular to
// the sizing / docking windows that are seen in Developer Studio.
class  CCJControlBar : public CControlBar
{
	DECLARE_DYNAMIC(CCJControlBar)

public:
	
	// Default constructor
	//
	CCJControlBar();

	// Virtual destructor
	//
	virtual ~CCJControlBar();

public:
	
	BOOL	m_bUnique;	// used by CCJControlBar and CCJSizeDockBar,
						// set to true when the controlbar is the 
						// only one docked on any given side.
	
	BOOL	m_bToFit;	// used by CCJControlBar and CCJSizeDockBar,
						// to determine border layouts depending on bar
						// state.

protected:			

	int				m_cxOffset;				// used to offset the horizontal client area
											// to allow room for the caption when docked.
	
	int				m_cyOffset;				// used to offset the vertical client area to 
											// allow room for the caption when docked.
	
	UINT			m_iAuxImage;			// used to determine which button icon to 
											// display when the control bar is minimized
											// or maximized depending on which side docked.

	UINT			m_popToolbarID;			// popup menu toolbar resource id, associates 
											// toolbar icons with the popup menu.

	UINT			m_popupMenuID;			// popup menu resource id
	CCJMenu			m_popupMenu;			// popup menu
	int				m_nPos;					// index of the pop-up menu contained in the menu

	BOOL			m_bOkToDrag;			// used to determine if a drag operation
											// should occur.

	BOOL			m_bMaximized;			// used to determine if the control bar has
											// been maximized.

	BOOL			m_bDragging;			// set to TRUE when a drag operation is in
											// process.


	BOOL			m_bButtons;				// user defined flag, set to TRUE if the
											// control bar should display buttons.

	CRect			m_rcTrack;				// used to determine rect of the tracker window.
	CSize			m_sizeDesired;			// the desired size of the control bar.
	CSize			m_sizeNormal;			// normal size of the control bar.
    CSize           m_sizeVert;				// vertical size
    CSize           m_sizeHorz;				// horizontal size
    CSize           m_sizeFloat;			// float size
	CPoint			m_ptCurDrag;			// cursor location during a drag operation.
	CPoint			m_ptStartDrag;			// cursor starting point during a drag operation.
	
	CWnd*			m_pChildWnd;			// CWnd pointer which is contains child window
											// that is displayed in the control bar, see
											// SetChild(CWnd* pWnd).

	CImageList		m_ImageList;			// contains the caption control buttons images.
	CCJCBarButton	m_btnClose;				// close button used in caption area.
	CCJCBarButton	m_btnMinim;				// minimize button used in the caption area.
	CToolTipCtrl	m_ToolTip;				// tooltip used for caption buttons.

	BOOL			m_bGripper;				// user defined flag, set to TRUE if a 
											// gripper should be drawn in the control 
											// bar, see ShowFrameControls(...).
    BOOL            m_bFlatGripper;			// user defined flag, set to TRUE if for a flat
											// Gripper look
    BOOL            m_bFlatBorder;			// user defined flag, set to TRUE if for a flat
											// Border look 
	BOOL			m_bAutoSave;
	CString			m_strSection;

public:
	DWORD GetDockedPos();
	void WriteRegistryInfo(CSize& size);
	DWORD GetRegistryInfo(CSize& size, DWORD dwStyle, UINT nID);
	void EnableAutoSave(BOOL bAutoSave=TRUE);

	// this member function is used to create a sizing / docking control bar.
	//
	virtual BOOL Create(
		// the parent window.
		CWnd* pParentWnd,
		// The ID of the control bar.
		UINT nID,
		// Points to a null-terminated character string that contains the 
		// control bar title.
		LPCTSTR lpszWindowName = NULL,
		// The default size the control bar is to be.
		CSize sizeDefault = CSize(200,100),
		// Specifies the window style attributes.
		DWORD dwStyle = CBRS_LEFT,
		// TRUE for flat style control bars.
		BOOL bFlatBorders = FALSE);

	// this member function is called by the control bar, and
	// can be overloaded in derived classes to return the rect
	// for the child window associated with the control bar.
	//
	virtual void GetChildRect(
		// reference to a CRect object which contains the size of
		// the child window associated with the control bar.
		CRect &rect);

	// this member function is called by the control bar to
	// determine the minimum size of the control bar should be 
	// when miminized or maximized.
	//
	int GetMinExt();

	// this member function returns TRUE if the control 
	// bar is docked on the left.
	//
	BOOL IsLeftDocked();

	// this member function returns TRUE if the control 
	// bar is docked on the right.
	//
	BOOL IsRightDocked();

	// this member function returns TRUE if the control 
	// bar is docked on the top.
	//
	BOOL IsTopDocked();

	// this member function returns TRUE if the control 
	// bar is docked on the bottom.
	//
	BOOL IsBottomDocked();

	// this member function returns TRUE if the control 
	// bar is docked horizontal.
	//
	BOOL IsHorzDocked();

	// this member function returns TRUE if the control 
	// bar is docked vertical.
	//
	BOOL IsVertDocked();

	// this member function is called by the control bar 
	// to set the normal size the control bar should be.
	//
	void SetNormalSize(const CSize &cs);

	// this member function is called by the control bar 
	// to return the control bar to its normal size.
	//
	void Normalize();

	// this member function is called by the control bar 
	// to minimize the control bar.
	//
	void Minimize();
	
	// this member function is called by the control bar 
	// to maximize the control bar.
	//
	void Maximize(int size);

	// this member function is to be called after creation to enable 
	// docking on the control bar.
	//
	void EnableDockingOnSizeBar(
		// Specifies which sides of the frame window can serve as docking sites 
		// for control bars. It can be one or more of the following: 
		//
		// CBRS_ALIGN_TOP   Allows docking at the top of the client area.
		// CBRS_ALIGN_BOTTOM   Allows docking at the bottom of the client area.
		// CBRS_ALIGN_LEFT   Allows docking on the left side of the client area.
		// CBRS_ALIGN_RIGHT   Allows docking on the right side of the client area.
		// CBRS_ALIGN_ANY   Allows docking on any side of the client area. 
		DWORD dwDockStyle);

	// this member function is used to show / hide the frame controls 
	// for the control bar.
	//
	void ShowFrameControls(
		// set to FALSE to hide the gripper.
		BOOL bGripper = TRUE,
		// set to FALSE to hide the frame buttons.
		BOOL bButtons = TRUE);

	// this member function is used to set the resource id for the popup menu used.
	//
	void SetMenuID(UINT popupMenuID, UINT popToolbarID=0, int nPos=0);

	// this member function returns the resource id of the context menu used.
	//
	UINT GetMenuID();

	void EnableFlatLook(BOOL bFlat=TRUE);

	// this member function is used to determine the width when docked vertical.
	//
	int GetVertSize();

	// this member function is used to determine the height when docked horizontal
	//
	int GetHorzSize();

protected:

	// this member function is called by the control bar to caluclate
	// the size of the gripper in the caption area.
	//
	CRect GetGripperRect();

	// this member function is called by the control bar to calculate the 
	// size of the buttons used in the caption area.
	//
	CRect GetButtonRect();

	// this member function is called by the control bar to draw
	// the gripper in the caption area.
	//
	void DrawGripper(CDC* pDC);

	// this member function is used to associate a child window 
	// that is to be displayed in the control bar.
	//
	void SetChild(CWnd* pWnd);

	// this member function is called by the control bar
	// during resizing of splitters.
	//
	void OnInvertTracker(const CRect& rect);

	// this member function is called by the control bar
	// to determine cursor position
	// for drag operations.
	//
	void HitTest(CPoint &point);

	// this member function is called by the control bar
	// to draw its borders.
	//
	virtual void DrawBorders(CDC *pDC, CRect &rect);

	// this member function is called by the control bar
	// to perform paint operations.
	//
	void EraseNonClient();

	// this member function is used to draw the new flat style control bar.
	//
    void DrawGriperCaption( CDC * pDC, LPRECT pRect );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJControlBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
 	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CCJControlBar)
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonClose();
	afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
	afx_msg void OnButtonMinimize();
	afx_msg void OnUpdateButtonMinimize(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

#define AFX_IDW_SIZEBAR_LEFT	AFX_IDW_DOCKBAR_LEFT	+ 4
#define AFX_IDW_SIZEBAR_RIGHT	AFX_IDW_DOCKBAR_RIGHT	+ 5
#define AFX_IDW_SIZEBAR_TOP		AFX_IDW_DOCKBAR_TOP		+ 6
#define AFX_IDW_SIZEBAR_BOTTOM	AFX_IDW_DOCKBAR_BOTTOM	+ 7

const DWORD dwSizeBarMap[4][2] =
{
	{ AFX_IDW_SIZEBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_SIZEBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_SIZEBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_SIZEBAR_RIGHT,    CBRS_RIGHT  },
};

/////////////////////////////////////////////////////////////////////////////
//	Inline Functions
/////////////////////////////////////////////////////////////////////////////

_CJXLIB_INLINE void CCJControlBar::EnableAutoSave(BOOL bAutoSave)
	{ m_bAutoSave = bAutoSave; }

_CJXLIB_INLINE int CCJControlBar::GetVertSize()
	{ ASSERT(::IsWindow(m_hWnd)); return m_sizeDesired.cx; }

_CJXLIB_INLINE int CCJControlBar::GetHorzSize()
	{ ASSERT(::IsWindow(m_hWnd)); return m_sizeDesired.cy; }

_CJXLIB_INLINE UINT CCJControlBar::GetMenuID()
	{ ASSERT(::IsWindow(m_hWnd)); return m_popupMenuID; }

_CJXLIB_INLINE void CCJControlBar::SetMenuID(UINT popupMenuID, UINT popToolbarID, int nPos)
	{ ASSERT(::IsWindow(m_hWnd)); m_popupMenuID=popupMenuID; m_popToolbarID=popToolbarID; m_nPos=nPos; }

_CJXLIB_INLINE void CCJControlBar::ShowFrameControls(BOOL bGripper, BOOL bButtons)
	{ ASSERT(::IsWindow(m_hWnd)); m_bGripper=bGripper; m_bButtons=bButtons; }

_CJXLIB_INLINE void CCJControlBar::SetNormalSize(const CSize &cs)
	{ ASSERT(::IsWindow(m_hWnd)); m_sizeDesired=m_sizeNormal=cs; }

_CJXLIB_INLINE BOOL CCJControlBar::IsVertDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return (IsLeftDocked() || IsRightDocked()); }

_CJXLIB_INLINE BOOL CCJControlBar::IsHorzDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return (IsTopDocked() || IsBottomDocked()); }

_CJXLIB_INLINE BOOL CCJControlBar::IsBottomDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return m_pDockBar?(m_pDockBar->GetDlgCtrlID()==AFX_IDW_SIZEBAR_BOTTOM):FALSE; }

_CJXLIB_INLINE BOOL CCJControlBar::IsTopDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return m_pDockBar?(m_pDockBar->GetDlgCtrlID()==AFX_IDW_SIZEBAR_TOP):FALSE; }

_CJXLIB_INLINE BOOL CCJControlBar::IsRightDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return m_pDockBar?(m_pDockBar->GetDlgCtrlID()==AFX_IDW_SIZEBAR_RIGHT):FALSE; }

_CJXLIB_INLINE BOOL CCJControlBar::IsLeftDocked()
	{ ASSERT(::IsWindow(m_hWnd)); return m_pDockBar?(m_pDockBar->GetDlgCtrlID()==AFX_IDW_SIZEBAR_LEFT):FALSE; }

_CJXLIB_INLINE void CCJControlBar::SetChild(CWnd *pWnd)
	{ ASSERT(::IsWindow(m_hWnd)); m_pChildWnd=pWnd; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CJCONTROLBAR_H__

