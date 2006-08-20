// CJFlatComboBox.cpp : implementation file
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
 * $Revision: 11 $
 * $Archive: /CodeJockey/CJLibrary/CJFlatComboBox.cpp $
 *
 * $History: CJFlatComboBox.cpp $
 * 
 * *****************  Version 11  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 9:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Gilberto Botaro [seafox@techno.com.br] - Added code for auto
 * completion.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 10:44p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed code ommisions.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 8:42p
 * Updated in $/CodeJockey/CJ60Lib
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
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/22/99    Time: 4:26p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed accelerator problem reported by YOSH YAMASHITA
 * (yosh@muj.biglobe.ne.jp) with Matt Weagle's solution of overloading
 * PreTranslateMessage().
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/02/99    Time: 4:25p
 * Updated in $/CodeJockey/CJ60Lib
 * Renamed class from CJComboBox to CCJFlatComboBox, changed DRAWSTATE to
 * enum, removed method Offset(), and replaced with a direct call to
 * ::GetSystemMetrics().
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/02/98   Time: 4:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Added OnSetFocus(), OnKillFocus() and modified OnPaint() so the control
 * remains beveled until it actually looses focus - (Vachik
 * Hovhannissian).
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/05/98   Time: 4:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed offset bug to use GetSystemMetrics(), instead of hard coded
 * values -
 * Todd Brannam (tbrannam@msn.com).
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/02/98   Time: 4:23p
 * Created in $/CodeJockey/CJ60Lib
 * First release version.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJFlatComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJFlatComboBox

CCJFlatComboBox::CCJFlatComboBox()
{
	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;
	m_bAutoComp = FALSE;
}

CCJFlatComboBox::~CCJFlatComboBox()
{
}

IMPLEMENT_DYNAMIC(CCJFlatComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CCJFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCJFlatComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,  OnKillFocus)
	ON_CONTROL_REFLECT(CBN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJFlatComboBox message handlers

void CCJFlatComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1,10,NULL);
	OnTimer(1);
	CComboBox::OnMouseMove(nFlags, point);
}

void CCJFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CCJFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CCJFlatComboBox::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if (m_bLBtnDown==TRUE) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo(pressed, ::GetSysColor(COLOR_BTNSHADOW),
				::GetSysColor(COLOR_BTNHIGHLIGHT));
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo(normal, ::GetSysColor(COLOR_BTNFACE),
				::GetSysColor(COLOR_BTNFACE));
			m_bPainted = FALSE;
		}
		return;
	}

	// On mouse over, show raised.
	else {
		if (m_bPainted == TRUE)
			return;
		else {
			m_bPainted = TRUE;
			DrawCombo(raised, ::GetSysColor(COLOR_BTNSHADOW),
				::GetSysColor(COLOR_BTNHIGHLIGHT));
		}
	}
	
	CComboBox::OnTimer(nIDEvent);
}

void CCJFlatComboBox::OnPaint() 
{
	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);
	
	Default();

	CPoint pt;
	GetCursorPos(&pt);

	CRect rcItem;
	GetWindowRect(&rcItem);

	if ((rcItem.PtInRect(pt)) || m_bHasFocus)
		DrawCombo(raised, ::GetSysColor(COLOR_BTNSHADOW),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
	else
		DrawCombo(normal, ::GetSysColor(COLOR_BTNFACE),
			::GetSysColor(COLOR_BTNFACE));
}

void CCJFlatComboBox::DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	CRect rcItem;
	GetClientRect(&rcItem);
	CDC* pDC = GetDC();
	
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1,1);
	
	if (!IsWindowEnabled()) {
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
	}
	
	else {
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
			::GetSysColor(COLOR_BTNFACE));
	}

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-::GetSystemMetrics(SM_CXHTHUMB);;
	pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_BTNFACE));
	
	// Cover up normal 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_BTNFACE));
	
	if (!IsWindowEnabled()) {
		return;
	}

	switch (eState)
	{
	case normal:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		rcItem.left -= 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		break;

	case raised:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNSHADOW));
		break;

	case pressed:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		rcItem.OffsetRect(1,1);
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNSHADOW),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		break;
	}

	ReleaseDC(pDC);
}

void CCJFlatComboBox::OnSetFocus()
{
	m_bHasFocus = TRUE;
	DrawCombo(raised, ::GetSysColor(COLOR_BTNSHADOW),
		::GetSysColor(COLOR_BTNHIGHLIGHT));
}

void CCJFlatComboBox::OnKillFocus() 
{
	m_bHasFocus = FALSE;
	DrawCombo(normal, ::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_BTNFACE));	
}

BOOL CCJFlatComboBox::PreTranslateMessage(MSG* pMsg) 
{
	if(IsWin95()) // alexander Golde [alex@viona.de]
	{
		// Make sure that the keystrokes continue to the appropriate handlers
		if (pMsg->message == WM_KEYDOWN || pMsg->message==WM_KEYUP)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		
		// Catch the Alt key so we don't choke if focus 
		// is going to an owner drawn button
		if (pMsg->message == WM_SYSCHAR)
			return TRUE;
	}
		
	return CComboBox::PreTranslateMessage(pMsg);
}

void CCJFlatComboBox::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (!m_bAutoComp) 
		return;
	
	// Get the text in the edit box
	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();
	
	// Currently selected range
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
	
	// Search for, and select in, and string in the combo box that is prefixed
	// by the text in the edit box
	if (SelectString(-1, str) == CB_ERR)
	{
		SetWindowText(str);		// No text selected, so restore what was there before
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);	//restore cursor postion
	}
	
	// Set the text selection as the additional text that we have added
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}

BOOL CCJFlatComboBox::IsWin95()
{
	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);
	GetVersionEx( &info );
	return ((VER_PLATFORM_WIN32_WINDOWS == info.dwPlatformId) && (info.dwMinorVersion == 0));
}
