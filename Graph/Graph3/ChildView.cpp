// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Graph3.h"
#include "ChildView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_Point[0][0].x=200;
	m_Point[0][0].y=100-200;
	m_Point[0][0].z=0;
	m_Point[0][1].x=200;
	m_Point[0][1].y=250-200;
	m_Point[0][1].z=0;
	m_Point[0][2].x=350;
	m_Point[0][2].y=350-200;
	m_Point[0][2].z=0;
	m_Point[0][3].x=300;
	m_Point[0][3].y=500-200;
	m_Point[0][3].z=0;

	m_Drawing=false;

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.BitBlt(0,0,m_Cx,m_Cy,&m_MemDC,0,-1*m_Cy/2,SRCCOPY);
//	dc.BitBlt(0,0,m_Cx,m_Cy,&m_MemDC,-1*m_Cx/2,-1*m_Cy/2,SRCCOPY);
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_Cx=::GetSystemMetrics(SM_CXSCREEN);
	m_Cy=::GetSystemMetrics(SM_CYSCREEN);
	CPaintDC dc(this);
	HBRUSH brush=(HBRUSH)::GetStockObject(BLACK_BRUSH);
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,m_Cx,m_Cy);
	m_MemDC.SelectObject(&m_Bitmap);
//	m_MemDC.SetMapMode(MM_ISOTROPIC);
//	m_MemDC.SetWindowExt(m_Cx,m_Cy);
	m_MemDC.SetViewportExt(m_Cx,m_Cy);
//	m_MemDC.SetWindowOrg(m_Cx/2,m_Cy/2);
	m_MemDC.SetViewportOrg(0,m_Cy/2);
//	m_MemDC.SetViewportOrg(m_Cx/2,m_Cy/2);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,-1*m_Cy/2,m_Cx,m_Cy,PATCOPY);
//	m_MemDC.TextOut(0,0,"Press ENTER to continue",30);

	// 初始化花瓶各线段
	float delta;
	int i,j;
	float t1,t2;
	for(j=1,delta=360.0/CIRCLE;j<CIRCLE;j++)
	{
		for(i=0;i<LINE;i++)
		{
			t1=cos(delta*PI/180.0);
			t2=sin(delta*PI/180.0);
			m_Point[j][i].x=m_Point[0][i].x*t1+m_Point[0][i].z*t2-400*t1+400;
			m_Point[j][i].y=m_Point[0][i].y;
			m_Point[j][i].z=m_Point[0][i].x*(-1)*t2+m_Point[0][i].z*t1+400*t2;
			if(i!=0)
			{	
				Line(&m_MemDC,RGB(255,0,0),m_Point[j][i].x,m_Point[j][i].y,m_Point[j][i-1].x,m_Point[j][i-1].y);
			}
			Line(&m_MemDC,RGB(255,0,0),m_Point[j][i].x,m_Point[j][i].y,m_Point[j-1][i].x,m_Point[j-1][i].y);
		}
		delta+=(float)360/CIRCLE;
	}
	for(i=1;i<LINE;i++)
	{
		Line(&m_MemDC,RGB(255,0,0),m_Point[0][i].x,m_Point[0][i].y,m_Point[CIRCLE-1][i].x,m_Point[CIRCLE-1][i].y);
		Line(&m_MemDC,RGB(255,0,0),m_Point[0][i].x,m_Point[0][i].y,m_Point[0][i-1].x,m_Point[0][i-1].y);
	}
	Line(&m_MemDC,RGB(255,0,0),m_Point[0][0].x,m_Point[0][0].y,m_Point[CIRCLE-1][0].x,m_Point[CIRCLE-1][0].y);


	return 0;
}

void CChildView::Line(CDC* pdc,COLORREF clr,float x1,float y1,float x2,float y2)
{
	CPen pen(PS_SOLID,0,clr);
	CPen* oldpen;
	oldpen=pdc->SelectObject(&pen);
	pdc->MoveTo((int)x1,(int)y1);
	pdc->LineTo((int)x2,(int)y2);
	pdc->SelectObject(oldpen);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar = VK_RETURN)
	{
		if(!m_Drawing)
		{
			SetTimer(1,1000,NULL);
			m_Drawing=true;
		}
		else
		{
			KillTimer(1);
			m_Drawing=false;
		}
	}
//	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static float delta=10.0;
	int i,j;
	float t1,t2;
	Clear(&m_MemDC,RGB(0,0,0));

	t1=cos(delta*PI/180.0);
	t2=sin(delta*PI/180.0);
	float temp;
	for(j=0;j<CIRCLE;j++)
		for(i=0;i<LINE;i++)
		{
			temp=m_Point[j][i].y;
			m_Point[j][i].y=temp*t1-m_Point[j][i].z*t2;
			m_Point[j][i].z=temp*t2+m_Point[j][i].z*t1;
			if(i!=0)
				Line(&m_MemDC,RGB(255,0,0),m_Point[j][i].x,m_Point[j][i].y,m_Point[j][i-1].x,m_Point[j][i-1].y);
			if(j!=0)
				Line(&m_MemDC,RGB(255,0,0),m_Point[j][i].x,m_Point[j][i].y,m_Point[j-1][i].x,m_Point[j-1][i].y);
		}
	for(i=0;i<LINE;i++)
	{
		Line(&m_MemDC,RGB(255,0,0),m_Point[0][i].x,m_Point[0][i].y,m_Point[CIRCLE-1][i].x,m_Point[CIRCLE-1][i].y);
	}
	Invalidate(FALSE);
//	CWnd ::OnTimer(nIDEvent);
//	KillTimer(1);
}

void CChildView::Clear(CDC* pdc,COLORREF clrBack)
{
	CBrush brush(clrBack);
	CBrush* oldbrush;
	oldbrush=m_MemDC.SelectObject(&brush);
	m_MemDC.PatBlt(0,-1*m_Cy/2,m_Cx,m_Cy,PATCOPY);
//	m_MemDC.TextOut(0,0,"Press ENTER to continue",30);
	m_MemDC.SelectObject(oldbrush);
}