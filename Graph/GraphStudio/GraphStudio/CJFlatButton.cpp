// CJFlatButton.cpp : implementation file
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
 * $Date: 6/23/99 12:04a $
 * $Revision: 11 $
 * $Archive: /CodeJockey/CJLibrary/CJFlatButton.cpp $
 *
 * $History: CJFlatButton.cpp $
 * 
 * *****************  Version 11  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:04a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:15a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:27p
 * Updated in $/CodeJockey/CJLibrary
 * Cleaned up icon drawing function.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 7/14/99    Time: 9:58p
 * Updated in $/CodeJockey/CJLibrary
 * Re-wrote class to handle icons, text and bitmaps and added additional
 * functionality which was needed by the CCJCaption class.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:23p
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 3/13/99    Time: 4:24p
 * Updated in $/CodeJockey/CJ60Lib
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
 * Updated in $/CodeJockey/CJ60Lib
 * Code clean up and re-organization, renamed CCJButton to CCJFlatButton.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 4:23p
 * Created in $/CodeJockey/CJ60Lib
 * Initial re-write and release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJFlatButton.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJFlatButton

CCJFlatButton::CCJFlatButton()
{
	m_pFont		= NULL;
	m_hPushed	= NULL;
	m_bLBtnDown = FALSE;
	m_bFlatLook = TRUE;
	m_bPainted	= FALSE;

	m_clrHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = ::GetSysColor(COLOR_3DDKSHADOW);
	m_clrNormal = ::GetSysColor(COLOR_BTNFACE);
	m_clrTextGy = ::GetSysColor(COLOR_GRAYTEXT);
	m_clrTextNm = ::GetSysColor(COLOR_BTNTEXT);

	m_hCursor	= AfxGetApp()->LoadCursor( IDC_HANDCUR );
	UpdateFont();
}

CCJFlatButton::~CCJFlatButton()
{
}


BEGIN_MESSAGE_MAP(CCJFlatButton, CButton)
	//{{AFX_MSG_MAP(CCJFlatButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJFlatButton message handlers

void CCJFlatButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bFlatLook)
	{
		SetTimer(1, 10, NULL);
		OnTimer(1);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CCJFlatButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CCJFlatButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	CButton::OnLButtonUp(nFlags, point);
}

void CCJFlatButton::OnTimer(UINT nIDEvent) 
{
	CRect rcItem;
	GetWindowRect(rcItem);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(( m_bLBtnDown == TRUE ) || ( !rcItem.PtInRect( ptCursor )))
	{
		KillTimer(1);

		if (m_bPainted == TRUE) {
			InvalidateRect (NULL);
		}

		m_bPainted = FALSE;
		return;
	}

	// On mouse over, show raised button.
	else if(( m_bFlatLook ) && ( !m_bPainted )) {
		CDC* pDC = GetDC();
		GetClientRect(rcItem);
		pDC->Draw3dRect( rcItem, m_clrHilite, m_clrShadow );
		m_bPainted = TRUE;
		ReleaseDC(pDC);
	}
	
	CButton::OnTimer(nIDEvent);
}

void CCJFlatButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	m_clrHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = ::GetSysColor(COLOR_3DDKSHADOW);
	m_clrNormal = ::GetSysColor(COLOR_BTNFACE);
	m_clrTextGy = ::GetSysColor(COLOR_GRAYTEXT);
	m_clrTextNm = ::GetSysColor(COLOR_BTNTEXT);
}

void CCJFlatButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	// define some temporary variables.
	CDC*  pDC	 = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;
	int   nState = lpDrawItemStruct->itemState;

	// repaint the background.
	pDC->FillSolidRect( rcItem, m_clrNormal );

	// Draw flat button border.
	if( m_bFlatLook ) {
		pDC->Draw3dRect( rcItem, (nState & ODS_SELECTED)?m_clrShadow:m_clrNormal,
			(nState & ODS_SELECTED)?m_clrHilite:m_clrNormal );
	}
	
	// Draw normal border.
	else {
		pDC->Draw3dRect( rcItem, (nState & ODS_SELECTED)?m_clrDkShad:m_clrHilite,
			(nState & ODS_SELECTED)?m_clrHilite:m_clrDkShad);
		rcItem.DeflateRect(1,1);
		pDC->Draw3dRect( rcItem, (nState & ODS_SELECTED)?m_clrShadow:m_clrNormal,
			(nState & ODS_SELECTED)?m_clrNormal:m_clrShadow);
	}
		
	// Set the background mode to transparent.
	pDC->SetBkMode( TRANSPARENT );
	
	// draw the button text, icon or bitmap.
	DrawButtonText(pDC, nState, rcItem);
	DrawButtonIcon(pDC, nState, rcItem);
	DrawButtonBitmap(pDC, nState, rcItem);
}

void CCJFlatButton::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem)
{
	// Get the window text, return if empty.
	CString strWindowText; GetWindowText(strWindowText);
	if( strWindowText.IsEmpty( ))
		return;

	if( m_pFont == NULL ) {
		m_pFont = &m_Font;
	}

	CRect rc = rcItem;
	rc.DeflateRect( 2,0 );

	// Set the text color and select the button font.
	pDC->SetTextColor(( nState & ODS_DISABLED )?m_clrTextGy:m_clrTextNm);
	CFont *oldFont = pDC->SelectObject( m_pFont );
	
	// if the button is selected.
	if( nState & ODS_SELECTED ) {
		rc.OffsetRect(1,1);
	}

	// draw the text, and select the original font.
    pDC->DrawText( strWindowText, -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	pDC->SelectObject( oldFont );
}

void CCJFlatButton::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
{
	// get the handle to the icon (if any) associated
	// with this button.
	HICON hIcon = GetIcon();
	if( hIcon == NULL ) {
		return;
	}

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	
	int left   = (rcWnd.Width()-m_sizeIcon.cx)/2;
	int right  = left+m_sizeIcon.cx;
	int top    = (rcWnd.Height()-m_sizeIcon.cy)/2;
	int bottom = top+m_sizeIcon.cy;

	// determine the size of the icon to be drawn.
	CRect rc(left,top,right,bottom);

	// if the button is selected.
	if( nState & ODS_SELECTED )
	{
		if( m_hPushed ) {
			hIcon = m_hPushed;
		}
		else {
			rc.OffsetRect(1,1);
		}
	}

	// draw the icon associated with this button.
	pDC->DrawState( rc.TopLeft(), rc.Size(), hIcon, 
		( nState & ODS_DISABLED )?DSS_DISABLED:DSS_NORMAL, (HBRUSH)NULL );
}

void CCJFlatButton::DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem)
{
	// get the handle to the bitmap (if any) associated
	// with this button.
	HBITMAP hBitmap = GetBitmap();
	if( hBitmap == NULL ) {
		return;
	}

	CRect rc = rcItem;

	// if the button is selected.
	if( nState & ODS_SELECTED ) {
		rcItem.OffsetRect(1,1);
	}

	// draw the bitmap associated with this button.
	pDC->DrawState( rc.TopLeft(), rc.Size(), hBitmap, 
		( nState & ODS_DISABLED )?DSS_DISABLED:DSS_NORMAL, (HBRUSH)NULL );
}


BOOL CCJFlatButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor( m_hCursor );
	return TRUE;
}

void CCJFlatButton::UpdateFont()
{
	m_Font.DeleteObject();
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	VERIFY(m_Font.CreateFontIndirect(&ncm.lfMessageFont));
}
