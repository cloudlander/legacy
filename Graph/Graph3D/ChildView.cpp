// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Graph3D.h"
#include "ChildView.h"
#include "MainFrm.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int Cx,Cy;
#define PI  3.1415926
/*
void Dummy()
{
	int i,j;
	CMatrix m1(4,4,MATRIX_FLOAT);
	CMatrix m2(4,4,MATRIX_FLOAT);
	CMatrix m3(4,4,MATRIX_FLOAT);
	m1.SetAt(0,0,1);
	m1.SetAt(1,1,1);
	m1.SetAt(2,2,1);
	m1.SetAt(3,0,-40);
	m1.SetAt(3,1,-40);
	m1.SetAt(3,2,-40);
	m1.SetAt(3,3,1);

	m2.SetAt(0,0,1);
	m2.SetAt(1,1,1);
	m2.SetAt(2,2,1);
	m2.SetAt(3,3,1);


	m3=m1*m2*m1;
	for(i=0;i<4;i++)
		for( j=0;j<4;j++)
			TRACE("%f\t",m3.GetAt(i,j));
	TRACE("\n");
	for(i=0;i<4;i++)
		for( j=0;j<4;j++)
			TRACE("%f\t",m1.GetAt(i,j));
	TRACE("\n");
	for( i=0;i<4;i++)
		for( j=0;j<4;j++)
			TRACE("%f\t",m2.GetAt(i,j));

}

void DebugMatrix(CMatrix& temp)
{
	for(int i=0;i<temp.GetLineDim();i++){
		for(int j=0;j<temp.GetColDim();j++)
			TRACE("%f\t",temp.GetAt(i,j));
		TRACE("\n");
	}
	TRACE("\n");

}
*/

////////////////////////////////////////////////////////////////////////////
// CChildView
IMPLEMENT_DYNCREATE(CChildView, CWnd)

CChildView::CChildView():m_Trans(4,4,MATRIX_FLOAT),
m_LineX(4,4,MATRIX_FLOAT),m_LineXR(4,4,MATRIX_FLOAT),
m_LineY(4,4,MATRIX_FLOAT),m_LineYR(4,4,MATRIX_FLOAT),
m_LineZ(4,4,MATRIX_FLOAT),m_LineZR(4,4,MATRIX_FLOAT)
{
	m_Point[0].x=-300;
	m_Point[0].y=200;
	m_Point[0].z=150;
	m_Point[1].x=-300;
	m_Point[1].y=-200;
	m_Point[1].z=150;
	m_Point[2].x=300;
	m_Point[2].y=-200;
	m_Point[2].z=150;
	m_Point[3].x=300;
	m_Point[3].y=200;
	m_Point[3].z=150;

	m_Point[4].x=300;
	m_Point[4].y=-200;
	m_Point[4].z=-150;
	m_Point[5].x=300;
	m_Point[5].y=200;
	m_Point[5].z=-150;
	m_Point[6].x=-300;
	m_Point[6].y=200;
	m_Point[6].z=-150;
	m_Point[7].x=-300;
	m_Point[7].y=-200;
	m_Point[7].z=-150;
/*
	m_LineX.SetIdentity();
	m_LineXR.SetIdentity();
	m_LineY.SetIdentity();
	m_LineYR.SetIdentity();
	m_LineZ.SetIdentity();
	m_LineZR.SetIdentity();

	m_LineX.SetAt(3,2,175);
	m_LineXR.SetAt(3,2,-175);
	m_LineY.SetAt(3,2,175);
	m_LineYR.SetAt(3,2,-175);
	m_LineZ.SetAt(3,2,175);
	m_LineZR.SetAt(3,2,-175);
*/
	m_Umin=-1800;
	m_Umax=1800;
	m_Vmin=-1800;
	m_Vmax=1800;
//  视平面中心坐标
	m_VRP.x=0;
	m_VRP.y=0;
	m_VRP.z=700;
//  透视中心
	m_COP.x=0;
	m_COP.y=0;
	m_COP.z=-10;
  
	m_VPN.x=0;
	m_VPN.y=0;
	m_VPN.z=-1;

	m_VUP.x=0;
	m_VUP.y=1;
	m_VUP.z=-1;
	
	m_F=-1800;
	m_B=1800;
	
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
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
	dc.BitBlt(0,0,Cx,Cy,&m_MemDC,-1*Cx/2,-1*Cy/2,SRCCOPY);
//	dc.BitBlt(0,0,m_Cx,m_Cy,&m_MemDC,0,0,SRCCOPY);
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	m_Cx=m_Cy=400;
	CPaintDC dc(this);
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,Cx,Cy);
	m_MemDC.SelectObject(&m_Bitmap);


	m_MemDC.SetViewportExt(Cx,Cy);

	m_MemDC.SetViewportOrg(Cx/2,Cy/2);
	Clear(RGB(0,0,0));
	Draw();
	return 0;
}

void CChildView::Clear(COLORREF clr)
{
	CBrush brush(clr);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(-Cx/2,-Cy/2,Cx,Cy,PATCOPY);
//	m_MemDC.PatBlt(0,0,m_Cx,m_Cy,PATCOPY);

}

void CChildView::Draw()
{	
	Clear(RGB(0,0,0));

	CreateMatrix();
	CMatrix temp(1,4,MATRIX_FLOAT);
	CMatrix tempPoint(1,4,MATRIX_FLOAT);
	for(int i=0;i<8;i++)
	{
		tempPoint=m_Point[i];
//		DebugMatrix(tempPoint);
		temp=tempPoint * m_Trans;
//		DebugMatrix(temp);
		m_DrawPoint[i].x=temp.GetAt(0,0)/temp.GetAt(0,3);	
		m_DrawPoint[i].y=temp.GetAt(0,1)/temp.GetAt(0,3);
	}
	DrawLines(RGB(0,255,0));
	Invalidate(FALSE);
}

void CChildView::DrawLines(COLORREF clr)
{
	DrawOneLine(m_DrawPoint[0].x,m_DrawPoint[0].y,m_DrawPoint[3].x,m_DrawPoint[3].y,clr);
	DrawOneLine(m_DrawPoint[1].x,m_DrawPoint[1].y,m_DrawPoint[2].x,m_DrawPoint[2].y,clr);
	DrawOneLine(m_DrawPoint[4].x,m_DrawPoint[4].y,m_DrawPoint[7].x,m_DrawPoint[7].y,clr+200);
	DrawOneLine(m_DrawPoint[5].x,m_DrawPoint[5].y,m_DrawPoint[6].x,m_DrawPoint[6].y,clr+200);

	DrawOneLine(m_DrawPoint[1].x,m_DrawPoint[1].y,m_DrawPoint[7].x,m_DrawPoint[7].y,clr-10);
	DrawOneLine(m_DrawPoint[2].x,m_DrawPoint[2].y,m_DrawPoint[4].x,m_DrawPoint[4].y,clr-10);
	DrawOneLine(m_DrawPoint[3].x,m_DrawPoint[3].y,m_DrawPoint[5].x,m_DrawPoint[5].y,clr-10);
	DrawOneLine(m_DrawPoint[0].x,m_DrawPoint[0].y,m_DrawPoint[6].x,m_DrawPoint[6].y,clr-10);

	DrawOneLine(m_DrawPoint[0].x,m_DrawPoint[0].y,m_DrawPoint[1].x,m_DrawPoint[1].y,clr);
	DrawOneLine(m_DrawPoint[2].x,m_DrawPoint[2].y,m_DrawPoint[3].x,m_DrawPoint[3].y,clr);
	DrawOneLine(m_DrawPoint[4].x,m_DrawPoint[4].y,m_DrawPoint[5].x,m_DrawPoint[5].y,clr+200);
	DrawOneLine(m_DrawPoint[6].x,m_DrawPoint[6].y,m_DrawPoint[7].x,m_DrawPoint[7].y,clr+200);
}

void CChildView::DrawOneLine(float x1,float y1,float x2,float y2,COLORREF clr)
{
	CPen pen(PS_SOLID,0,clr);
	CPen* oldpen;
	oldpen=m_MemDC.SelectObject(&pen);
	m_MemDC.MoveTo((int)x1,(int)y1);
	m_MemDC.LineTo((int)x2,(int)y2);
	m_MemDC.SelectObject(oldpen);
}

void CChildView::SetCOP(float x,float y,float z)
{
	m_COP.x=x;
	m_COP.y=y;
	m_COP.z=z;
}

void CChildView::SetVPN(float x,float y,float z)
{
	m_VPN.x=x;
	m_VPN.y=y;
	m_VPN.z=z;
}

void CChildView::SetVUP(float x,float y,float z)
{
	m_VUP.x=x;
	m_VUP.y=y;
	m_VUP.z=z;
}

void CChildView::SetVRP(float x,float y,float z)
{
	m_VRP.x=x;
	m_VRP.y=y;
	m_VRP.z=z;
}

void CChildView::CreateMatrix()
{
	CMatrix Tper(4,4,MATRIX_FLOAT);
	Tper.SetIdentity();
	Tper.SetAt(3,0,-(m_COP.x+m_VRP.x));
	Tper.SetAt(3,1,-(m_COP.y+m_VRP.y));
	Tper.SetAt(3,2,-(m_COP.z+m_VRP.z));
//	DebugMatrix(Tper);
	
	Point3D vpn,u,v;
	float temp;
	temp=sqrt(m_VPN.x*m_VPN.x + m_VPN.y*m_VPN.y + m_VPN.z*m_VPN.z);
	vpn.x=m_VPN.x/temp;
	vpn.y=m_VPN.y/temp;
	vpn.z=m_VPN.z/temp;
	
	temp=m_VUP.x*vpn.x + m_VUP.y*vpn.y + m_VUP.z*vpn.z;

	v.x=m_VUP.x-vpn.x*temp;
	v.y=m_VUP.y-vpn.y*temp;
	v.z=m_VUP.z-vpn.z*temp;
	temp=sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x/=temp;
	v.y/=temp;
	v.z/=temp;

	u.x=vpn.y*v.z-vpn.z*v.y;
	u.y=vpn.z*v.x-vpn.x*v.z;
	u.z=vpn.x*v.y-vpn.y*v.x;
	CMatrix Trch(4,4,MATRIX_FLOAT);
	Trch.SetAt(0,0,u.x);
	Trch.SetAt(0,1,v.x);
	Trch.SetAt(0,2,vpn.x);
	Trch.SetAt(1,0,u.y);
	Trch.SetAt(1,1,v.y);
	Trch.SetAt(1,2,vpn.y);
	Trch.SetAt(2,0,u.z);
	Trch.SetAt(2,1,v.z);
	Trch.SetAt(2,2,vpn.z);
	Trch.SetAt(3,3,1);
//	DebugMatrix(Trch);

	CMatrix TempMatrix(Tper*Trch);
//	DebugMatrix(TempMatrix);
	CMatrix vrp(1,4,MATRIX_FLOAT);
	vrp.SetAt(0,0,m_VRP.x);
	vrp.SetAt(0,1,m_VRP.y);
	vrp.SetAt(0,2,m_VRP.z);
	vrp.SetAt(0,3,1);
	vrp=vrp*TempMatrix;
//	DebugMatrix(vrp);
	float a2,b2;
	CMatrix Tsh(4,4,MATRIX_FLOAT);
	Tsh.SetIdentity();
	if(vrp.GetAt(0,2) > 0.001 || vrp.GetAt(0,2)< -0.001)
	{
		a2=-((vrp.GetAt(0,0)+(m_Umax+m_Umin)/2)/vrp.GetAt(0,2));
		b2=-((vrp.GetAt(0,1)+(m_Vmax+m_Vmin)/2)/vrp.GetAt(0,2));	
		Tsh.SetAt(2,0,a2);
		Tsh.SetAt(2,1,b2);
	}
//	DebugMatrix(Tsh);

	CMatrix Tsper(4,4,MATRIX_FLOAT);
	CMatrix Mper(4,4,MATRIX_FLOAT);
	float sx,sy,sz,d;
	sz=1/(vrp.GetAt(0,2)+m_B);
	sx=2*vrp.GetAt(0,2)/(m_Umax-m_Umin) * sz;
	sy=2*vrp.GetAt(0,2)/(m_Vmax-m_Vmin) * sz;
//	d=1/(vrp.GetAt(0,2) * sz);
	d=1/vrp.GetAt(0,2);
	Tsper.SetAt(0,0,sx);
	Tsper.SetAt(1,1,sy);
	Tsper.SetAt(2,2,sz);
	Tsper.SetAt(3,3,1);
	Mper.SetIdentity();
	Mper.SetAt(2,3,d);
	Mper.SetAt(3,3,0);
//	DebugMatrix(Tsper);
//	DebugMatrix(Mper);

//	DebugMatrix(m_Trans);
//	m_Trans=TempMatrix * Tsh * Tsper * Mper;
	m_Trans=TempMatrix * Tsh  * Mper;
//	DebugMatrix(m_Trans);
}




void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	float data;
	Point3D a;
	CMatrix temp(1,4,MATRIX_FLOAT);
	CMatrix trans(4,4,MATRIX_FLOAT);
	trans.SetIdentity();
	switch(nChar)
 	{
	case  'w':
		m_COP.z+=5;
		break;
	case  'x':
		m_COP.z-=5;
		break;
	case  'b':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(10*PI/180.0);
		trans.SetAt(1,1,data);
		trans.SetAt(2,2,data);
		data=sin(10*PI/180.0);
		trans.SetAt(1,2,data);
		trans.SetAt(2,1,-1*data);
		temp=m_VRP;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;
	case 'B':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(-10*PI/180.0);
		trans.SetAt(1,1,data);
		trans.SetAt(2,2,data);
		data=sin(-10*PI/180.0);
		trans.SetAt(1,2,data);
		trans.SetAt(2,1,-data);
		temp=m_VRP;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
//		m_COP.x=temp.GetAt(0,0)-m_VRP.x;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineX * trans * m_LineXR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;
	case 'n':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(10*PI/180.0);
		trans.SetAt(0,0,data);
		trans.SetAt(2,2,data);
		data=sin(10*PI/180.0);
		trans.SetAt(0,2,-data);
		trans.SetAt(2,0,data);
		temp=m_VRP;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
//		m_COP.x=temp.GetAt(0,0)-m_VRP.x;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;
	case 'N':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(-10*PI/180.0);
		trans.SetAt(0,0,data);
		trans.SetAt(2,2,data);
		data=sin(-10*PI/180.0);
		trans.SetAt(0,2,data);
		trans.SetAt(2,0,-data);
		temp=m_VRP;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
//		m_COP.x=temp.GetAt(0,0)-m_VRP.x;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineY * trans * m_LineYR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;
	case  'm':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(10*PI/180.0);
		trans.SetAt(0,0,data);
		trans.SetAt(1,1,data);
		data=sin(10*PI/180.0);
		trans.SetAt(0,1,data);
		trans.SetAt(1,0,-data);
		temp=m_VRP;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
//		m_COP.x=temp.GetAt(0,0)-m_VRP.x;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;
	case 'M':
		a.x=m_VRP.x+m_COP.x;
		a.y=m_VRP.y+m_COP.y;
		a.z=m_VRP.z+m_COP.z;
		data=cos(-10*PI/180.0);
		trans.SetAt(0,0,data);
		trans.SetAt(1,1,data);
		data=sin(-10*PI/180.0);
		trans.SetAt(0,1,data);
		trans.SetAt(1,0,-data);
		temp=m_VRP;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VRP.x=temp.GetAt(0,0);
		m_VRP.y=temp.GetAt(0,1);
		m_VRP.z=temp.GetAt(0,2);
		temp=a;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
//		m_COP.x=temp.GetAt(0,0)-m_VRP.x;
//		m_COP.y=temp.GetAt(0,1)-m_VRP.y;
//		m_COP.z=temp.GetAt(0,2)-m_VRP.z;
		temp=m_VUP;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VUP.x=temp.GetAt(0,0);
		m_VUP.y=temp.GetAt(0,1);
		m_VUP.z=temp.GetAt(0,2);
		temp=m_VPN;
//		temp=temp * m_LineZ * trans * m_LineZR;
		temp=temp*trans;
		m_VPN.x=temp.GetAt(0,0);
		m_VPN.y=temp.GetAt(0,1);
		m_VPN.z=temp.GetAt(0,2);
		break;

 	}	
	Draw();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)	
	{
	case VK_UP:
		m_COP.y-=5;
		break;
	case  VK_DOWN:
		m_COP.y+=5;
		break;
	case VK_RIGHT:
 		m_COP.x+=5;
 		break;
	case  VK_LEFT:
 		m_COP.x-=5;
		break;
	}
	Draw();
//	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}
