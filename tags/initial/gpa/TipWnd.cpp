// TipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "GPA2.h"
#include "TipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipWnd

CTipWnd::CTipWnd()
{
}

CTipWnd::~CTipWnd()
{
}


BEGIN_MESSAGE_MAP(CTipWnd, CWnd)
	//{{AFX_MSG_MAP(CTipWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTipWnd message handlers

void CTipWnd::SetStaticText(char *s)
{
	str = s;
	Invalidate();
}

void CTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
/*
	CRect rc;
	GetClientRect(&rc);
	DWORD clr = GetSysColor(COLOR_WINDOW);
	COLORREF cc = RGB(GetRValue(clr),GetGValue(clr),GetBValue(clr));
	CBrush b;
	b.Attach(((HBRUSH)GetSysColorBrush(COLOR_WINDOW)));
	dc.FillRect(&rc,&b);
	b.Detach();
*/
	dc.TextOut(rc.right/4,rc.bottom/3,str);
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CTipWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;
   cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}



void CTipWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GetClientRect(&rc);
	
}

void CTipWnd::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	
	delete this;
	// TODO: Add your message handler code here
	
}
