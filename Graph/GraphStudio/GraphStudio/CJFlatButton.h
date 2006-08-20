// CJFlatButton.h : header file
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
 * $Date: 7/25/99 10:00p $
 * $Revision: 10 $
 * $Archive: /CodeJockey/Include/CJFlatButton.h $
 *
 * $History: CJFlatButton.h $
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:17p
 * Updated in $/CodeJockey/Include
 * Cleaned up inline functions, and import/export macro so that class will
 * be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 7/14/99    Time: 9:58p
 * Updated in $/CodeJockey/Include
 * Re-wrote class to handle icons, text and bitmaps and added additional
 * functionality which was needed by the CCJCaption class.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 6/09/99    Time: 8:45p
 * Updated in $/CodeJockey/Include
 * cleaned up _CJXLIB_INLINE functions, added validation assert.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:23p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 3/13/99    Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Patrick Bergeron [patb@softimage.com] fixed the following bugs:
 * 
 * Moving the cursor over the flat button doesn't raise the button right
 * away. We have to wait for the first timer message to arrive, which is a
 * bit annoying. It gives the impression that the system isn't very
 * responsive. So in the OnMouseMove method, we call OnTimer(1) to force
 * the initial update of the button.
 * 
 * Sometimes, a CJFlatButton would stay in a raised state when moving the
 * mouse quickly over the client area of a few CJFlatButtons. The reason
 * turned out to be the use of a static (ahem, global) variable
 * "bPainted". The simple fix for this was to put bPainted as a member
 * variable ("m_bPainted") and use this member variable instead.
 * 
 * Flat buttons are now much more responsive and behave correctly in all
 * cases.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Code clean up and re-organization, renamed CCJButton to CCJFlatButton.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 4:23p
 * Created in $/CodeJockey/Include
 * Initial re-write and release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJFLATBUTTON_H__
#define __CJFLATBUTTON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CCJFlatButton is a CButton derived class used to create flat buttons.
// this class can only be used with the BS_OWNERDRAW style bit set.
class  CCJFlatButton : public CButton
{

public:
	
	// Default constructor
	//
	CCJFlatButton();

	// Virtual destructor
	//
	virtual ~CCJFlatButton();

protected:

	BOOL		m_bMouseOver;
	BOOL		m_bLBtnDown;	// true if left mouse button is pressed
	BOOL		m_bFlatLook;	// true for flat buttons see DisableFlatLook()
	CSize		m_sizeIcon;		// size of the icon if any associated with the button
	COLORREF	m_clrHilite;	// set to ::GetSysColor(COLOR_BTNHIGHLIGHT)
	COLORREF	m_clrShadow;	// set to ::GetSysColor(COLOR_BTNSHADOW)
	COLORREF	m_clrDkShad;	// set to ::GetSysColor(COLOR_3DDKSHADOW)
	COLORREF	m_clrNormal;	// set to ::GetSysColor(COLOR_BTNFACE)
	COLORREF	m_clrTextGy;	// set to ::GetSysColor(COLOR_GRAYTEXT)
	COLORREF	m_clrTextNm;	// set to ::GetSysColor(COLOR_BTNTEXT)
	BOOL		m_bPainted;		// used during paint operations
	HICON		m_hPushed;
	CFont*		m_pFont;
	CFont		m_Font;			// default font created by control if m_pFont is NULL
	HCURSOR		m_hCursor;

public:
	void UpdateFont();

	void SetButtonFont(CFont* pFont);

	// this member function is used to disable or enable the flat
	// look for the button
	//
	void DisableFlatLook(
		// set to true to disable the flat look.
		BOOL bDisable = FALSE);

	// this memeber function is used to associate an icon with the
	// flat button.
	//
	void SetIcon(
		// handle to icon to be associated with button.
		HICON hIcon,
		// desired size of the icon, must be smaller than 
		// button size for best results.
		CSize size);

	// this memeber function is used to associate an icon with the
	// flat button.
	//
	void SetIcon(
		// handle to icon to be associated with button.
		HICON hIcon,
		// handle of the pushed icon to be associated with button.
		HICON hPushed,
		// desired size of the icon, must be smaller than 
		// button size for best results.
		CSize size);

protected:

	// this memeber funcion is called by the button during paint
	// operations.
	//
	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem);
	virtual void DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem);
	virtual void DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJFlatButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CCJFlatButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//	Inline Functions
/////////////////////////////////////////////////////////////////////////////

_CJXLIB_INLINE void CCJFlatButton::DisableFlatLook(BOOL bDisable)
	{ ASSERT(::IsWindow(m_hWnd)); m_bFlatLook = bDisable; Invalidate(); }

_CJXLIB_INLINE void CCJFlatButton::SetIcon(HICON hIcon, CSize size)
	{ ASSERT(::IsWindow(m_hWnd)); m_sizeIcon = size; ::DestroyIcon( CButton::SetIcon(hIcon) ); }

_CJXLIB_INLINE void CCJFlatButton::SetIcon(HICON hIcon, HICON hPushed, CSize size)
	{ ASSERT(::IsWindow(m_hWnd)); m_hPushed = hPushed; SetIcon(hIcon, size); }

_CJXLIB_INLINE void CCJFlatButton::SetButtonFont(CFont *pFont)
	{ ASSERT(::IsWindow(m_hWnd)); m_pFont = pFont; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CJFLATBUTTON_H__
