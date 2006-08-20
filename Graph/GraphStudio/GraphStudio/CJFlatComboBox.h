// CJFlatComboBox.h : header file
//
// Copyright � 1998-99 Kirk W. Stowell
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
// "Portions Copyright � 1998-99 Kirk Stowell" must be included in the startup 
// banner, "About" box or printed documentation. This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 7/25/99 10:00p $
 * $Revision: 13 $
 * $Archive: /CodeJockey/Include/CJFlatComboBox.h $
 *
 * $History: CJFlatComboBox.h $
 * 
 * *****************  Version 13  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 12  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 11  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:17p
 * Updated in $/CodeJockey/Include
 * Cleaned up inline functions, and import/export macro so that class will
 * be imported when linked to, and exported at compile time.
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 9:24p
 * Updated in $/CodeJockey/Include
 * Gilberto Botaro [seafox@techno.com.br] - Added code for auto
 * completion.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 10:44p
 * Updated in $/CodeJockey/Include
 * Fixed code ommisions.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 8:42p
 * Updated in $/CodeJockey/Include
 * Danilo Lax [Danilo_Lax@www2.alpeng.com] - The control is painted flat
 * when the mouse leaves the box, even if the control has the focus, now
 * checking for the focus in OnTimer(.
 * 
 * Removed, unhandled msgs are processed by
 * CComboBox::PreTranslateMessage(pMsg). Used reflected msgs
 * BN_GETFOCUS/CBN_KILLFOCUS instead of WM_GETFOCUS/WM_KILLFOCUS.
 * 
 * ::GetFocus() doesn't return CFlatComboBox::m_hWnd, even if the control
 * has the focus (don't know why, maybe ::GetFocus() only returns the
 * handle to the EditField part of the ComboBox), used flag m_bHasFocus
 * instead.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:26p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/22/99    Time: 4:26p
 * Updated in $/CodeJockey/Include
 * Fixed accelerator problem reported by YOSH YAMASHITA
 * (yosh@muj.biglobe.ne.jp) with Matt Weagle's solution of overloading
 * PreTranslateMessage().
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/02/99    Time: 4:25p
 * Updated in $/CodeJockey/Include
 * Renamed class from CJComboBox to CCJFlatComboBox, changed DRAWSTATE to
 * enum, removed method Offset(), and replaced with a direct call to
 * ::GetSystemMetrics().
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/02/98   Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Added OnSetFocus(), OnKillFocus() and modified OnPaint() so the control
 * remains beveled until it actually looses focus - (Vachik
 * Hovhannissian).
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/05/98   Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Fixed offset bug to use GetSystemMetrics(), instead of hard coded
 * values -
 * Todd Brannam (tbrannam@msn.com).
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/02/98   Time: 4:23p
 * Created in $/CodeJockey/Include
 * First release version.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJFLATCOMBOBOX_H__
#define __CJFLATCOMBOBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CCJFlatComboBox is a CComboBox derived class which is simular to
// the flat combobox as seen in the Microsoft office products.
class  CCJFlatComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CCJFlatComboBox)

public:
	
	// Default constructor
	//
	CCJFlatComboBox();

	// Virtual destructor
	//
	virtual ~CCJFlatComboBox();

protected:

	BOOL m_bLBtnDown;	// TRUE if left mouse button is pressed
	BOOL m_bPainted;	// used during paint operations
	BOOL m_bHasFocus;	// TRUE if control has focus
	BOOL m_bAutoComp;	// Used by Autocompleting.

	// enum used to determine the state the combo box should be
	//
	enum STATE { normal = 1, raised = 2, pressed = 3 };

public:
	BOOL IsWin95();

	// Call this member function to enable auto completion.
	//
	void EnableAutoCompletion(BOOL bEnable=TRUE);

protected:

	// this member function is called by the combo box whenever a paint
	// operation should occur.
	//
	void DrawCombo(enum STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJFlatComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CCJFlatComboBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus();
	afx_msg void OnKillFocus();
	afx_msg void OnEditUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//	Inline Functions
/////////////////////////////////////////////////////////////////////////////

_CJXLIB_INLINE void CCJFlatComboBox::EnableAutoCompletion(BOOL bEnable/*=TRUE*/)
	{ ASSERT(::IsWindow(m_hWnd)); m_bAutoComp = bEnable; }

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CJCOMBOBOX_H__
