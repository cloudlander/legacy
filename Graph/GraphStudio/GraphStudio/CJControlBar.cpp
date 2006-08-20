// CJControlBar.cpp : implementation file
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
 * $Date: 7/25/99 12:30a $
 * $Revision: 18 $
 * $Archive: /CodeJockey/CJLibrary/CJControlBar.cpp $
 *
 * $History: CJControlBar.cpp $
 * 
 * *****************  Version 18  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 17  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 16  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:23p
 * Updated in $/CodeJockey/CJLibrary
 * Added derived button class to handle control bar caption button
 * updates. Cleaned up inline functions, and import/export macro so that
 * class will be imported when linked to, and exported at compile time.
 * Added save and restore control bar size and state functionality.
 * 
 * *****************  Version 15  *****************
 * User: Kirk Stowell Date: 6/11/99    Time: 8:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Slight modification made to popup menu code.
 * 
 * *****************  Version 14  *****************
 * User: Kirk Stowell Date: 6/09/99    Time: 8:33p
 * Updated in $/CodeJockey/CJ60Lib
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
 * *****************  Version 13  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 8:33p
 * Updated in $/CodeJockey/CJ60Lib
 * Removed Invalidate() from OnWindowPosChanged(..) and OnEraseBkgnd(..)
 * which seemed to cause painting problems.
 * 
 * *****************  Version 12  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 9:52p
 * Updated in $/CodeJockey/CJ60Lib
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
 * Updated in $/CodeJockey/CJ60Lib
 * Modified context menu to use Brent Corkum's BCMenu (CCJMenu).
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 12:52p
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 3/13/99    Time: 12:54p
 * Updated in $/CodeJockey/CJ60Lib
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
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed memory leak with button tooltips.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 3/10/99    Time: 12:48p
 * Updated in $/CodeJockey/CJ60Lib
 * Added AfxRegisterWndClass() to create method to handle double clicks.
 * Thanks to Takehiko Mizoguti [mizoguti@m2.sys.to.casio.co.jp] for some
 * thoughts on this.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 2/28/99    Time: 12:47p
 * Updated in $/CodeJockey/CJ60Lib
 * Calls default wnd proc if no context menu is defined Gonzalo Pereyra
 * [persys@adinet.com.uy]
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/31/99    Time: 12:48p
 * Updated in $/CodeJockey/CJ60Lib
 * Removed extra line (typo) from OnLButtonUp(). Thanks to Ioannis
 * Stamatopoulos (ystamat@mail.datamedia.gr) for pointing this out.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/12/99    Time: 12:46p
 * Updated in $/CodeJockey/CJ60Lib
 * Total class re-write, added multiple/side-by-side controlbar docking.
 * No longer uses CSizingControlBar base class.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/22/98   Time: 12:47p
 * Updated in $/CodeJockey/CJ60Lib
 * Modified set cursor to display normal size cursor when static linked.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/20/98   Time: 12:45p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed problem with gripper and buttons disappearing when docking
 * toggled. Overloaded IsFloating() method from base class.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/17/98   Time: 12:44p
 * Created in $/CodeJockey/CJ60Lib
 * Initial re-write and release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJControlBar.h"
#include "CJDockContext.h"
#include "CJSizeDockBar.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRIP_INTRASPACING		1
#define GRIP_SIZE				3
#define GRIP_START				8

#define TRACK_BORDER_SIZE		4

enum BTN_IMG_INDEX
{
	HIDE_CONTRLBAR, 
	MAXI_H_DISABLE,
	MAXI_H_ENABLED,
	MAXI_V_DISABLE,
	MAXI_V_ENABLED,
	NORM_H_DISABLE,
	NORM_H_ENABLED,
	NORM_V_DISABLE,
	NORM_V_ENABLED
};

void CCJCBarButton::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
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
	pDC->DrawState( rc.TopLeft(), rc.Size(), hIcon, DSS_NORMAL, (HBRUSH)NULL );
}

/////////////////////////////////////////////////////////////////////////////
// CCJControlBar

CCJControlBar::CCJControlBar()
{
	m_popupMenuID		= 0;
	m_popToolbarID		= 0;
	m_nPos				= 0;
	m_bToFit			= FALSE;
	m_bOkToDrag			= FALSE;
	m_bDragging			= FALSE;
	m_bMaximized		= FALSE;
	m_bUnique			= FALSE;
	m_bGripper			= TRUE;
	m_bButtons			= TRUE;
    m_bFlatGripper      = FALSE;			// Yiannhs Ver 6.06 Start Up With Flat Gripper
    m_bFlatBorder       = FALSE;			// Yiannhs Start Up With Flat Border
    
    m_sizeVert          = CSize(200,100);   // Yiannhs to test different rectangles
    m_sizeHorz          = CSize(200,100);
    m_sizeFloat         = CSize(200,100);
	m_sizeDesired		= CSize(200,100);
	m_sizeNormal		= CSize(200,100);
	m_cxOffset			= 5;
	m_cyOffset			= 3;

	// Create the image list used by frame buttons.
	m_ImageList.Create(IDB_BUTTON_IMAGES,
		10, 1, RGB(255,255,255));

	m_pChildWnd			= NULL;
	m_strSection		= _T("");
	m_bAutoSave			= FALSE;
}

CCJControlBar::~CCJControlBar()
{
	m_ImageList.DeleteImageList();
}

IMPLEMENT_DYNAMIC(CCJControlBar, CControlBar)

BEGIN_MESSAGE_MAP(CCJControlBar, CControlBar)
	//{{AFX_MSG_MAP(CCJControlBar)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDC_BUTTON_HIDE, OnButtonClose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HIDE, OnUpdateButtonClose)
	ON_COMMAND(IDC_BUTTON_MINI, OnButtonMinimize)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MINI, OnUpdateButtonMinimize)
	ON_WM_RBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//	ON_WM_ERASEBKGND()

/////////////////////////////////////////////////////////////////////////////
// CCJControlBar overrides

void CCJControlBar::EnableFlatLook(BOOL bFlat)
{
	ASSERT(::IsWindow(m_hWnd));
	m_bFlatGripper = m_bFlatBorder = bFlat;
	m_btnClose.DisableFlatLook( bFlat );
	m_btnMinim.DisableFlatLook( bFlat );
	Invalidate();
}

BOOL CCJControlBar::Create(CWnd *pParentWnd, UINT nID, LPCTSTR lpszWindowName, CSize sizeDefault, DWORD dwStyle, BOOL bFlatBorders)
{
    ASSERT_VALID(pParentWnd);   // must have a parent

	if( m_bAutoSave ) {
		dwStyle = GetRegistryInfo(sizeDefault, dwStyle, nID);
	}

    // Set initial control bar style.
    SetBarStyle(dwStyle & CBRS_ALL|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|CBRS_FLOAT_MULTI);

    dwStyle &= ~CBRS_ALL;
    dwStyle |= WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
    
	m_sizeDesired = m_sizeNormal = sizeDefault;
    m_sizeHorz = m_sizeVert = m_sizeFloat = sizeDefault; // Yiannhs : Setup the new sizes as well

	// Set flat control bar style.
	m_bFlatGripper = m_bFlatBorder = bFlatBorders;

    // Yiannhs this seems a better way than mine, to register the double clicks class.
    // I have just added the redraw flags i was using in ver 6.06
	CString wndClass = ::AfxRegisterWndClass(
		CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, 0, ::GetSysColorBrush(COLOR_BTNFACE));

    // Yiannhs : Why use an empty rectangle when we have a default size
	return CWnd::Create( wndClass, lpszWindowName, dwStyle,
		CRect(CW_USEDEFAULT, CW_USEDEFAULT, sizeDefault.cx, sizeDefault.cx), pParentWnd, nID );
}

BOOL CCJControlBar::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bButtons)
		m_ToolTip.RelayEvent(pMsg);
	return CControlBar::PreTranslateMessage(pMsg);
}

void CCJControlBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
	if (m_bButtons)
	{
		BTN_IMG_INDEX eAuxImage = HIDE_CONTRLBAR;
		BOOL bEnable = FALSE;

		if (IsHorzDocked())
		{
			if (m_bUnique) {
				m_ToolTip.UpdateTipText (IDS_EXPAND, &m_btnMinim);
				eAuxImage = MAXI_H_DISABLE;
				bEnable = FALSE;
			}

			else if (!m_bMaximized) {
				m_ToolTip.UpdateTipText (IDS_EXPAND, &m_btnMinim);
				eAuxImage = MAXI_H_ENABLED;
				bEnable = TRUE;
			}

			else {
				m_ToolTip.UpdateTipText (IDS_CONTRACT, &m_btnMinim);
				eAuxImage = NORM_H_ENABLED;
				bEnable = TRUE;
			}
		}
		
		else if(IsVertDocked())
		{
			if (m_bUnique) {
				m_ToolTip.UpdateTipText (IDS_EXPAND, &m_btnMinim);
				eAuxImage = MAXI_V_DISABLE;
				bEnable = FALSE;
			}

			else if (!m_bMaximized) {
				m_ToolTip.UpdateTipText (IDS_EXPAND, &m_btnMinim);
				eAuxImage = MAXI_V_ENABLED;
				bEnable = TRUE;
			}

			else {
				m_ToolTip.UpdateTipText (IDS_CONTRACT, &m_btnMinim);
				eAuxImage = NORM_V_ENABLED;
				bEnable = TRUE;
			}
		}

		if (eAuxImage != (BTN_IMG_INDEX)m_iAuxImage) {
			m_iAuxImage = eAuxImage;
			m_btnMinim.SetIcon(m_ImageList.ExtractIcon(m_iAuxImage), CSize(10,10));
			m_btnMinim.EnableWindow(bEnable);
		}
	}
}

//1999-05-19 Yiannhs. 
CSize CCJControlBar::CalcDynamicLayout(int nLength, DWORD nMode)
{
	CSize sizeResult = m_sizeDesired;
	
    //1999-05-19 Yiannhs. Since We Get Here from different Places do some more calculations.
    //                    Give Visual Feedback to user.
    if( !nLength ) {
        CRect rect;
        GetWindowRect( rect );
        sizeResult = rect.Size();
        
        if( nMode & LM_HORZDOCK )
            if( !IsHorzDocked() )
                sizeResult = m_sizeHorz;
			else if( nMode & LM_VERTDOCK )
				if( !IsVertDocked() )
					sizeResult = m_sizeVert;
				else
					sizeResult = m_sizeFloat;           //Do not check if it is floating. Do it
				
				return( sizeResult );                   //do not go on
    }
    //1999-05-19    Yiannhs
	
	if( IsFloating( ))
	{
		if (nMode == LM_HORZ) {
			if (nLength < 50)
				nLength = 50;
			m_sizeDesired.cx = nLength;
		}
		
		if (nMode == (LM_LENGTHY | LM_HORZ)) {
			if (nLength < 50)
				nLength = 50;
			m_sizeDesired.cy = nLength;
		}
		
		sizeResult = m_sizeDesired;
		
	}
	
	//1999-05-19    Yiannhs
	
	return sizeResult;
}

/////////////////////////////////////////////////////////////////////////////
// CCJControlBar message handlers

void CCJControlBar::OnNcPaint() 
{
	EraseNonClient();
}

void CCJControlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	m_iAuxImage = (UINT)-1;
	GetClientRect(&rc);
	CRect rect;
	GetChildRect(rect);
	dc.ExcludeClipRect(rect);
	DrawBorders(&dc,rc);
}

void CCJControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	HitTest(point);	
	CRect rectDock;
	if( m_bDragging )
	{
		ASSERT(GetParent() != NULL);

		GetParent()->GetClientRect(&rectDock);
		ClientToScreen(&rectDock);
		ClientToScreen(&point);
		
		CFrameWnd *pFrame=GetDockingFrame();
		ASSERT(pFrame != NULL);

		pFrame -> ScreenToClient(&point);
		pFrame->ScreenToClient(&rectDock);

		point.x = point.x>rectDock.right ? rectDock.right:point.x;
		point.x = point.x<rectDock.left ? rectDock.left:point.x;
		point.y = point.y>rectDock.bottom ? rectDock.bottom:point.y;
		point.y = point.y<rectDock.top ? rectDock.top:point.y;

		OnInvertTracker(m_rcTrack);
		int deltaX = point.x-m_ptCurDrag.x;
		int deltaY = point.y-m_ptCurDrag.y;
		
		if(IsVertDocked()) {
			m_rcTrack.top += deltaY;
			m_rcTrack.bottom += deltaY;
		}

		else if (IsHorzDocked()) {
			m_rcTrack.left += deltaX;
			m_rcTrack.right += deltaX;
		}

		OnInvertTracker(m_rcTrack);
		m_ptCurDrag = point;
	}
	
	else {
	    // Yiannhs: Do not let default handler go on, if we have worked with the message
	    CControlBar::OnMouseMove(nFlags, point);
    }
}

BOOL CCJControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return (!m_bOkToDrag && !m_bDragging)?
		CControlBar::OnSetCursor(pWnd, nHitTest, message):FALSE;
}


//1999-05-19    Yiannhs
void CCJControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bDragging )
	{
	    ReleaseCapture();
	    OnInvertTracker(m_rcTrack);
//1999-05-19 Yiannhs. Moved the statement below
	    //m_bDragging = FALSE;        
		
        // Yiannhs: reduce unnessesary actions. Ok It works
 //1999-05-19 Yiannhs. Changed according to the statement above
        if( m_ptCurDrag == m_ptStartDrag ) {   
            m_bDragging = FALSE;            
            return;
        }

		if(IsVertDocked()) {
			int newHeight = m_rcTrack.top-m_ptStartDrag.y;
			m_sizeDesired.cy += newHeight;
			m_sizeDesired.cy = m_sizeDesired.cy > GetMinExt() ? m_sizeDesired.cy : GetMinExt();
            // Yiannhs: there is a comment in the previous version for change. This is the Line
            // This is probably for sizing with many control bars in dock bar
			// m_sizeDesired.cy = m_sizeDesired.cy > rectAvail.Height() ? rectAvail.Height() : m_sizeDesired.cy;

//1999-05-19 Yiannhs. Assign unused sizes
            m_sizeVert = m_sizeDesired;     
		}
	    
		else { //if (IsHorzDocked()) {  //1999-05-19. Floating control bars do not receive this message

			int newWidth = m_rcTrack.left-m_ptStartDrag.x;
			m_sizeDesired.cx += newWidth;
			m_sizeDesired.cx = m_sizeDesired.cx > GetMinExt() ? m_sizeDesired.cx : GetMinExt();
            // Yiannhs: there is a comment in the previous version for change. This is the Line
            // This is probably for sizing with many control bars in dock bar
	        // m_sizeDesired.cx = m_sizeDesired.cx > rectAvail.Width() ? rectAvail.Width() : m_sizeDesired.cx;

//1999-05-19 Yiannhs. Assign unused sizes
            m_sizeHorz = m_sizeDesired;     
	    }

//1999-05-19    Yiannhs
	    //((CCJSizeDockBar *)GetParent())->CalcSizeBarLayout();
        //resizing a control bar, means, that the DockBar should be normalized.
        //and normalize does the layout
        m_sizeNormal = m_sizeDesired;       //set the new normal size here
        ((CCJSizeDockBar *)GetParent())->Normalize( this );
	    m_bDragging = FALSE;        //flag off the drag action now. The movement has been complited
                                    //so as I can check it on change position
        CRect rc;                   // Now Use The Size of the control bar to fill the right sizes.
        GetWindowRect( rc );
        if( IsHorzDocked() ) 
            m_sizeNormal.cy = m_sizeHorz.cy = m_sizeDesired.cy = rc.Height();
        else
            m_sizeNormal.cx = m_sizeVert.cx = m_sizeDesired.cx = rc.Width();
//1999-05-19    Yiannhs

	}
	
	else {
        // Yiannhs: again don't go to default handler if we have already handled the message
        CControlBar::OnLButtonUp(nFlags, point);
    }
}

UINT CCJControlBar::OnNcHitTest(CPoint /*point*/)
{
	return HTCLIENT;
}

BOOL CCJControlBar::OnEraseBkgnd(CDC* pDC) 
{
	if (!CControlBar::OnEraseBkgnd(pDC))
		return FALSE;

	// Only want to draw a client rect when there is no
	// child present.
	if( m_pChildWnd == NULL )
	{
		CRect rect;
		GetChildRect(rect);
		rect.DeflateRect(1,1);
		// Yiannhs. use afzData.
		pDC->Draw3dRect(rect, afxData.clrBtnShadow, afxData.clrBtnHilite);
	}

	return TRUE;
}

int CCJControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_bButtons)
	{
		if(!m_btnClose.Create(NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP |
			BS_NOTIFY | BS_OWNERDRAW | BS_ICON,
			CRect(0,0,0,0), this, IDC_BUTTON_HIDE ))
		{
			TRACE0(_T("Unable to create close button\n"));
			return -1;
		}


		if(!m_btnMinim.Create(NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP |
			BS_NOTIFY | BS_OWNERDRAW | BS_ICON,
			CRect(0,0,0,0), this, IDC_BUTTON_MINI ))
		{
			TRACE0(_T("Unable to create minimize button\n"));
			return -1;
		}

        // Yiannhs Selective flat style interface. The difference is in adding the if statement
        if( !m_bFlatBorder && !m_bFlatGripper ) {
		    m_btnClose.DisableFlatLook();
		    m_btnMinim.DisableFlatLook();
        }

		m_btnClose.SetIcon(m_ImageList.ExtractIcon(0), CSize(10,10));

		// Create the ToolTip control.
		m_ToolTip.Create(this);
		m_ToolTip.Activate(TRUE);

		m_ToolTip.AddTool (&m_btnClose, IDS_HIDE);
		m_ToolTip.AddTool (&m_btnMinim, IDS_EXPAND);
	}
	
	return 0;
}


//1999-05-19 Yiannhs
void CCJControlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CControlBar::OnWindowPosChanged(lpwndpos);
	
	if (m_bButtons)
	{
		if (IsFloating())
		{
			m_btnClose.ShowWindow(SW_HIDE);
			m_btnMinim.ShowWindow(SW_HIDE);
		}
		
		else
		{
			m_btnClose.ShowWindow(SW_SHOW);
			m_btnMinim.ShowWindow(SW_SHOW);

			CRect rcClose(GetButtonRect());
			CRect rcMinim(GetButtonRect());

			if (IsHorzDocked()) {
				rcMinim.OffsetRect(0,14);
			}

			else {
				rcClose.OffsetRect(14,0);
			}

			m_btnClose.MoveWindow(rcClose);
			m_btnMinim.MoveWindow(rcMinim);
		}
	}

//1999-05-19 Yiannhs
    //Calculate Extra Size variables
    CSize tmpSz( lpwndpos->cx, lpwndpos->cy );  
	
	if( m_bAutoSave ) {
		WriteRegistryInfo(tmpSz);
	}

    if( IsFloating() ) {                

        m_sizeFloat = tmpSz;

    } else if( !m_bDragging ) {     //this is the reason I changed the [m_bDragging = FALSE] position above
  
        if( IsHorzDocked() ) {
            m_sizeHorz = tmpSz;
        } else {
            m_sizeVert = tmpSz;
        }

    }
//1999-05-19 Yiannhs

	if( m_pChildWnd->GetSafeHwnd( ))
	{
		CRect rc;
		GetChildRect(rc);
		rc.DeflateRect(1,1);
		m_pChildWnd->MoveWindow(rc);
	}
}

//1999-05-19 Yiannhs
void CCJControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // Yiannhs. When the bar is inside a mdiframe window, and click on area
    // the inner child does not cover, the whole area is grayed!!!
    
	if( IsFloating( )) //see Tab ControlBar
	{
		// If the window is floating, we want to 
		// bring it up to the top.
		SetFocus();
		SetWindowPos( NULL, 0,0,0,0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
        return;
	}

	if (m_bOkToDrag)
	{
		CFrameWnd *pFrame=GetDockingFrame();
		GetClientRect(m_rcTrack);
		
		if (IsVertDocked()) {
            // Yiannhs 
            m_rcTrack.top = m_rcTrack.bottom - TRACK_BORDER_SIZE -2;
			m_rcTrack.bottom-=2;
		}
//1999-05-19 Yiannhs. do not call one more function
		else { //if (IsHorzDocked()) {
            // Yiannhs 
			m_rcTrack.left = m_rcTrack.right - TRACK_BORDER_SIZE -2;
			m_rcTrack.right-=2;
		}
		
		ClientToScreen(&m_rcTrack);
		pFrame->ScreenToClient(&m_rcTrack);
		ClientToScreen(&point);
		pFrame->ScreenToClient(&point);
		
		m_ptStartDrag = point;
		m_ptCurDrag = point;
		
		SetCapture();
		m_bDragging = TRUE;
		OnInvertTracker(m_rcTrack);
	}

	else if (m_pDockBar) {
		if (OnToolHitTest(point, NULL) == -1) {
			ClientToScreen(&point);
//1999-05-26 Yiannhs
//          Handle the message loop before CDockContext
		    MSG msg;
		    ValidateRect( NULL );	//Make sure this control bar will not ask for Paint messages later
            HWND    hwnd = NULL;
	        while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
	        {   //ask for messages in any window of the application, and remove those
                DispatchMessage( &msg );
                if( msg.hwnd != this->m_hWnd ) {
                    hwnd = msg.hwnd;    //keep the window which receives paint messages
                    break;              //exit the loop
                }
            }
            
            if( hwnd ) {    //if a window is getting Paint messages work on it
                ::ValidateRect( hwnd, NULL );   //the window does not need painting
                ::ValidateRect( ::GetParent( hwnd ), NULL );    //Its parent window as well
                while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE)) {
                    //work on the Paint messages once more
                    ::TranslateMessage( &msg );
                    ::DispatchMessage( &msg ); 
                }
            }

//1999-05-26 Yiannhs

			((CCJDockContext*)m_pDockContext)->StartDragDockBar(point);
		}
	}

    // Yiannhs 
	else { 
		CControlBar::OnLButtonDown(nFlags, point);
	}
}

void CCJControlBar::OnButtonClose()
{
	GetDockingFrame()->ShowControlBar(this,FALSE,FALSE);
}

void CCJControlBar::OnUpdateButtonClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CCJControlBar::OnButtonMinimize()
{
	if (!m_bMaximized) {
		((CCJSizeDockBar *)GetParent())->Maximize(this);
		m_bMaximized = TRUE;
	}
	
	else {
		((CCJSizeDockBar *)GetParent())->Normalize(this);
		m_bMaximized = FALSE;
	}
}

void CCJControlBar::OnUpdateButtonMinimize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

/////////////////////////////////////////////////////////////////////////////
// CCJControlBar operations

void CCJControlBar::EnableDockingOnSizeBar(DWORD dwDockStyle)
{
	ASSERT(m_pDockContext == NULL);
	m_dwDockStyle = dwDockStyle;

	if (m_pDockContext == NULL)
		m_pDockContext = new CCJDockContext(this);

	if (m_hWndOwner == NULL)
		m_hWndOwner = ::GetParent(m_hWnd);
}

void CCJControlBar::Maximize(int size)
{
	m_sizeNormal = m_sizeDesired;
	if (IsHorzDocked()) {
		m_sizeDesired.cx = size;
	}

	else if (IsVertDocked()) {
		m_sizeDesired.cy = size;
	}

	m_bMaximized = TRUE;
}

//1999-05-19 Yiannhs. 
void CCJControlBar::Minimize()
{
//1999-05-19 Yiannhs. If the bar was previously maximized, do not
    //                    Change The Normal Size
    //if( !m_bMaximized )       //It didn't work as expected
//1999-05-19 Yiannhs. 

	m_sizeNormal = m_sizeDesired;

	if (IsHorzDocked()) {
		m_sizeDesired.cx = GetMinExt();
	}

	else if (IsVertDocked()) {
		m_sizeDesired.cy = GetMinExt();
	}

	m_bMaximized = FALSE;
}

void CCJControlBar::Normalize()
{
	if (IsHorzDocked()) {
		m_sizeDesired.cx = m_sizeNormal.cx;
	}

	else if (IsVertDocked()) {
		m_sizeDesired.cy = m_sizeNormal.cy;
	}

	m_bMaximized = FALSE;
}

int CCJControlBar::GetMinExt()
{
	int nRet = TRACK_BORDER_SIZE;

	if (m_bGripper || m_bButtons) {
		nRet += GRIP_START;
		nRet += GRIP_SIZE*2;
		nRet += GRIP_INTRASPACING*3+2;
	}

	else
		nRet += 2;

	return nRet;
}

void CCJControlBar::GetChildRect(CRect &rect)
{
	GetClientRect(&rect);
	
	if (!IsFloating())
	{
		if (IsVertDocked()) {
			rect.left	+= m_cxOffset;
			rect.right	-= m_cxOffset;
			rect.top	+= (GetMinExt()-5);
            //Yiannhs: if it is the last bar inside a dockbar, cover the whole space
            if( !m_bToFit )
                rect.bottom	-= (TRACK_BORDER_SIZE + 4);
            else
                rect.bottom	-= 4;
			//rect.bottom	-= (m_iTrackBorderSize+4);
			
		} else { //if(IsHorzDocked()) {  
            //Yianhhs: It doesn't float, so it is horizontally docked
			rect.left	+= (GetMinExt()-5);
            //Yiannhs: if it is the last bar inside a dockbar, cover the whole space
            if( !m_bToFit )
				rect.right	-= (TRACK_BORDER_SIZE + 4);
            else
                rect.right	-= 4;
            //rect.right	-= (m_iTrackBorderSize+4);
			rect.top	+= m_cyOffset;
			rect.bottom -= m_cyOffset;
		}
		
		if( rect.left > rect.right || rect.top > rect.bottom )
			rect = CRect(0,0,0,0);
	}
	else
	{
		rect.DeflateRect(2,2);
	}
}

void CCJControlBar::EraseNonClient()
{
	CWindowDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	
    dc.ExcludeClipRect(rectClient);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

    //Yiannhs: checking if this can be removed
	//dc.IntersectClipRect(rectWindow);
	//SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);
}

void CCJControlBar::DrawBorders(CDC *pDC, CRect &rect)
{
	CRect rc;
	rc.CopyRect(&rect);

	CRect rcTrack;
	rcTrack.CopyRect(&rc);

	if(IsVertDocked())
	{
		if (!m_bToFit) {
			rcTrack.top = rc.bottom - TRACK_BORDER_SIZE;
			rcTrack.top-=2;
			rcTrack.bottom-=1;

            //Yiannhs flat interface style
            if( !m_bFlatBorder )
			    pDC->Draw3dRect(rcTrack, afxData.clrBtnHilite, afxData.clrBtnShadow);
            else
                pDC->FillSolidRect(rcTrack, afxData.clrBtnFace);
		}
	}
	
	else if (IsHorzDocked())
	{
		if (!m_bToFit) {
			rcTrack.left = rc.right - TRACK_BORDER_SIZE;
			rcTrack.left-=2;
			rcTrack.right-=1;

            //Yiannhs flat interface style
            if( !m_bFlatBorder )
			    pDC->Draw3dRect(rcTrack, afxData.clrBtnHilite, afxData.clrBtnShadow);
            else
                pDC->FillSolidRect(rcTrack, afxData.clrBtnFace);
		}
	}

	DrawGripper(pDC);
}

void CCJControlBar::HitTest(CPoint &point)
{
	CRect rcWin;
	GetClientRect(&rcWin);
	HCURSOR hCur = ::LoadCursor(NULL,IDC_ARROW);
	BOOL bHit = TRUE;

    if( !m_bDragging ) { //yiannhs Check dragging !!
        //when dragging the control bars inside a SizeDockBar , the
        //dragging Pointer reverts to normal pointer again !!

	    if (IsVertDocked()) {
		    rcWin.top = rcWin.bottom - TRACK_BORDER_SIZE;
		    hCur = AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR);
		    bHit = rcWin.PtInRect(point) && !m_bToFit;
	    }
	    
	    else if (IsHorzDocked()) {
		    rcWin.left = rcWin.right - TRACK_BORDER_SIZE;
		    hCur = AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR);
		    bHit = rcWin.PtInRect(point) && !m_bToFit;	
	    }
    }

	SetCursor( bHit? hCur : ::LoadCursor(NULL,IDC_ARROW));
	m_bOkToDrag = bHit;
}

void CCJControlBar::OnInvertTracker(const CRect &rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	CFrameWnd *pFrame=GetDockingFrame();	
	CDC* pDC = pFrame->GetDC();
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	pFrame->ReleaseDC(pDC);
}

// Yiannhs this was taken from code in CodeGuru site. Draw gradient flat caption
// still needs some refinement

#define COLOR_WHITE		RGB(255,255,255)
#define COLOR_BLACK		RGB(0,0,0)
#define NCOLORSHADES	64		// this many shades in gradient

void CCJControlBar::DrawGriperCaption( CDC * pDC, LPRECT pRect )        //yiannhs
{
    ASSERT( pDC );
    ASSERT( pRect );
    if (!IsFloating() && m_bGripper)
    {   
        BOOL m_bIsHorz = IsHorzDocked();
        CRect TheRect( pRect );
        COLORREF clrBG = GetSysColor(COLOR_ACTIVECAPTION); // background color
        int cxCap = 0;
        int cyCap = 0;
        int rx = TheRect.left;
        int ry = TheRect.top;
        if( !m_bIsHorz ) {
            cxCap = TheRect.Width();
            cyCap = TheRect.Height();
        } else {
            cxCap = TheRect.Height();
            cyCap = TheRect.Width();
			//            rx = TheRect.top;
			//            ry = TheRect.left;
        }
		
		int r = GetRValue(clrBG);				// red..
		int g = GetGValue(clrBG);				// ..green
		int b = GetBValue(clrBG);				// ..blue color vals
		int w = cxCap;							// width of area to shade
		int xDelta= max(w/NCOLORSHADES,1);	    // width of one shade band
        int x = xDelta;						
        CBrush* pOldBrush = NULL;
        {
			CBrush brush(RGB(0,0,0));
			pOldBrush = pDC->SelectObject(&brush);
            if( !m_bIsHorz ) 
				pDC->PatBlt(rx, ry, x, cyCap, PATCOPY);
            else
                pDC->PatBlt(rx, ry, cyCap, x, PATCOPY);
            pDC->SelectObject(pOldBrush);
        }
        int w2  = w*w;							                    // w squared
		while( x < cxCap ) {                                       // paint bands right to left
			//x += xDelta;                                            // next band
			int wmx2 = (w-x)*(w-x);                                 // w minus x squared
            COLORREF clr = RGB( r-(r*wmx2)/w2, g-(g*wmx2)/w2, b-(b*wmx2)/w2 );
            {
				CBrush brush(clr);
                pDC->SelectObject(&brush);
                if( !m_bIsHorz ) 
					pDC->PatBlt(rx+x, ry, xDelta, cyCap, PATCOPY);
                else
                    pDC->PatBlt(rx, ry+x, cyCap, xDelta, PATCOPY);
                pDC->SelectObject(pOldBrush);
            }
            x += xDelta;
		}
        {
			CBrush brush(clrBG);
            pDC->SelectObject(&brush);
            
            if( !m_bIsHorz ) 
				pDC->PatBlt(rx+x, ry, cxCap-x, cyCap, PATCOPY);
            else
                pDC->PatBlt(rx, ry+x, cyCap, cxCap-x, PATCOPY);
        }
        pDC->SelectObject(pOldBrush);
    }
}//yiannhs new method ends

void CCJControlBar::DrawGripper(CDC *pDC)
{
    //Yiannhs. changed to make evaluations on top. Replaced with flat gradient caption
	if (!IsFloating() && m_bGripper)
	{
		// draw the gripper.
		CRect pRect(GetGripperRect());

        //Yiannhs New Lines
        CRect SecondGrip;
        SecondGrip.CopyRect( pRect ); 
        //Yiannhs: use the original beveled style gripper
		if (IsHorzDocked()) {
			SecondGrip.OffsetRect(4,0);
        } else {
			SecondGrip.OffsetRect(0,4);
        }

        //Yiannhs Selective Gripper look
        if( !m_bFlatGripper ) {       

            //Yiannhs: also updated to use AfxData
            pDC->FillSolidRect( pRect, afxData.clrBtnShadow  );
		    pDC->Draw3dRect( pRect, afxData.clrBtnHilite, afxData.clrBtnShadow );
        
            pDC->FillSolidRect( SecondGrip, afxData.clrBtnShadow );
		    pDC->Draw3dRect( SecondGrip, afxData.clrBtnHilite, afxData.clrBtnShadow );

        } else {    //Yiannhs: Use Flat Gripper

            CRect FlatRect( pRect.left, pRect.top, SecondGrip.right, SecondGrip.bottom);            
            if (IsHorzDocked()) {
			    FlatRect.DeflateRect(-2,0);
            } else {
			    FlatRect.DeflateRect(0,-2);
            }

            //if (IsHorzDocked()) {
            //    FlatRect.right += 6;
            //    FlatRect.bottom += 2;
            //} else {
            //    FlatRect.right += 2;
            //    FlatRect.bottom += 6;
            //}		

            DrawGriperCaption( pDC, FlatRect );
        }
	}
}

CRect CCJControlBar::GetButtonRect()
{
	CRect rect;
	GetClientRect(&rect);
	rect.OffsetRect(-rect.left,-rect.top);

	if (IsHorzDocked()) {
		rect.top	 += 3;
		rect.bottom = rect.top+12;
		rect.left  += 2;
		rect.right  = rect.left+12;
	}

	else {
		rect.right -= 19;
		rect.left   = rect.right-12;
		rect.top   += 3;
		rect.bottom = rect.top+12;
	}
	
	return rect;
}

CRect CCJControlBar::GetGripperRect()
{
	CRect rect;
	GetClientRect(&rect);
	rect.OffsetRect(-rect.left,-rect.top);

	if (IsHorzDocked()) {
		rect.DeflateRect(3,3);
		rect.left	+= 1;
		rect.right	= rect.left+3;
		rect.bottom	-= 1;
		rect.top	+= m_bButtons?30:1;
	}
	
	else {
		rect.DeflateRect(4,4);
		rect.top	 += 2;
		rect.bottom	 = rect.top+3;
		rect.right	 -= m_bButtons?30:2;
	}

	return rect;
}

void CCJControlBar::OnRButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// No menu was defined...
	if (!m_popupMenuID) {
		TRACE0(_T("Warning: No menu resource was associated with CCJControlBar.\n")); 
	}

	else
	{
		SetCapture();

		// Since we are using the main window as the
		// parent, we need to convert the point to screen
		// coordinates...
		CPoint pt = point;
		ClientToScreen(&pt);

		// Load the popup menu.
		VERIFY(m_popupMenu.LoadMenu(m_popupMenuID));
		CCJMenu* pPopup = (CCJMenu*)m_popupMenu.GetSubMenu(m_nPos);
		ASSERT(pPopup != NULL);

		// Load toolbar resource if any for menu icons.
		if (m_popToolbarID) {
			pPopup->LoadToolbar(m_popToolbarID);
		}
		
		//Yiannhs: When right clicking on a floating controlbar, the menu is inactive,
		//         this is due to the MiniFrame  apart from being a child is also
		//          a Frame Window. and only the main window handles the Bitmap menus
		//pWndPopupOwner = GetParentFrame();
		CWnd* pWndPopupOwner = GetParentFrame();    //this;
		if( IsFloating() )
			pWndPopupOwner = pWndPopupOwner->GetParentFrame();

		// Display the popup menu, use the main window as its parent.
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			pt.x, pt.y, pWndPopupOwner);

		m_popupMenu.DestroyMenu();

		ReleaseCapture();
	}
}

void CCJControlBar::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (lpMeasureItemStruct->CtlType == ODT_MENU) 
	{
		if (IsMenu((HMENU)lpMeasureItemStruct->itemID)) {
			CMenu* pMenu = CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			
			if (m_popupMenu.IsMenu(pMenu)) {
				m_popupMenu.MeasureItem(lpMeasureItemStruct);
				return;
			}
		}
	}
	
	CControlBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

LRESULT CCJControlBar::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	if (m_popupMenu.IsMenu(pMenu)) {
		return CCJMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	}

	return CControlBar::OnMenuChar(nChar, nFlags, pMenu);
}

void CCJControlBar::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CControlBar::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	if(!bSysMenu) {
		if (m_popupMenu.IsMenu(pPopupMenu)) {
			CCJMenu::UpdateMenu(pPopupMenu);
		}
	}
}

DWORD CCJControlBar::GetRegistryInfo(CSize& size, DWORD dwStyle, UINT nID)
{
	m_strSection.Format(_T("Bar State-Bar%d"), nID);
	BOOL bFloating = FALSE;
	CWinApp* pApp = AfxGetApp();
	size.cx = pApp->GetProfileInt( m_strSection, _T("CX"), size.cx );
	size.cy = pApp->GetProfileInt( m_strSection, _T("CY"), size.cy );
	bFloating = pApp->GetProfileInt( m_strSection, _T("Floating"), FALSE );

	if( !bFloating ) {
		DWORD dw = pApp->GetProfileInt( m_strSection, _T("Style"), dwStyle );
		if( dw != NULL ) {
			return dw;
		}
	}
	
	return dwStyle;
}

void CCJControlBar::WriteRegistryInfo(CSize& size)
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt( m_strSection, _T("CX"),		 size.cx	   );
	pApp->WriteProfileInt( m_strSection, _T("CY"),		 size.cy	   );
	pApp->WriteProfileInt( m_strSection, _T("Floating"), IsFloating()  );
	pApp->WriteProfileInt( m_strSection, _T("Style"),	 GetDockedPos());
}

DWORD CCJControlBar::GetDockedPos()
{
	if( IsLeftDocked( )) {
		return CBRS_ALIGN_LEFT;
	}
	else if( IsTopDocked( )) {
		return CBRS_ALIGN_TOP;
	}
	else if( IsRightDocked( )) {
		return CBRS_ALIGN_RIGHT;
	}
	else if( IsBottomDocked( )) {
		return CBRS_ALIGN_BOTTOM;
	}
	return NULL;
}
