// 3DView.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "3DView.h"
#include "CaptureBitmap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DView

IMPLEMENT_DYNCREATE(C3DView, CScrollView)

C3DView::C3DView():m_Trans(4,4),
m_LineX(4,4),m_LineXR(4,4),
m_LineY(4,4),m_LineYR(4,4),
m_LineZ(4,4),m_LineZR(4,4)
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
	m_Umin=-1400;
	m_Umax=1400;
	m_Vmin=-1400;
	m_Vmax=1400;

//  视平面中心坐标
	m_VRP.x=0;
	m_VRP.y=0;
	m_VRP.z=800;
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
	
	m_F=-100;
	m_B=100;

	Cx = 1024;
	Cy = 768;
}

C3DView::~C3DView()
{
}


BEGIN_MESSAGE_MAP(C3DView, CScrollView)
	//{{AFX_MSG_MAP(C3DView)
	ON_WM_CREATE()
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_COMMAND(ID_SHOW_SETUP, OnShowSetup)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SETUP, OnUpdateShowSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DView drawing

void C3DView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 1024;
	sizeTotal.cy = 768;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	pDC->BitBlt(0,0,Cx,Cy,&m_MemDC,-1*Cx/2,-1*Cy/2,SRCCOPY);
}

void C3DView::Clear(COLORREF clr)
{
	CBrush brush(clr);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(-Cx/2,-Cy/2,Cx,Cy,PATCOPY);

}

void C3DView::Draw()
{	
	Clear(RGB(0,0,0));

	CreateMatrix();
	CMatrix temp(1,4);
	CMatrix tempPoint(1,4);
	for(int i=0;i<8;i++)
	{
		tempPoint.SetAt(0,0,m_Point[i].x);
		tempPoint.SetAt(0,1,m_Point[i].y);
		tempPoint.SetAt(0,2,m_Point[i].z);
		tempPoint.SetAt(0,3,1.0);
		temp=tempPoint * m_Trans;
		m_DrawPoint[i].x=temp.GetAt(0,0)/temp.GetAt(0,3);	
		m_DrawPoint[i].y=temp.GetAt(0,1)/temp.GetAt(0,3);
	}
	DrawLines(RGB(0,255,0));
	Invalidate(FALSE);
}

void C3DView::DrawLines(COLORREF clr)
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

void C3DView::DrawOneLine(float x1,float y1,float x2,float y2,COLORREF clr)
{
	CPen pen(PS_SOLID,0,clr);
	CPen* oldpen;
	oldpen=m_MemDC.SelectObject(&pen);
	m_MemDC.MoveTo((int)x1,(int)y1);
	m_MemDC.LineTo((int)x2,(int)y2);
	m_MemDC.SelectObject(oldpen);
}

void C3DView::SetCOP(float x,float y,float z)
{
	m_COP.x=x;
	m_COP.y=y;
	m_COP.z=z;
}

void C3DView::SetVPN(float x,float y,float z)
{
	m_VPN.x=x;
	m_VPN.y=y;
	m_VPN.z=z;
}

void C3DView::SetVUP(float x,float y,float z)
{
	m_VUP.x=x;
	m_VUP.y=y;
	m_VUP.z=z;
}

void C3DView::SetVRP(float x,float y,float z)
{
	m_VRP.x=x;
	m_VRP.y=y;
	m_VRP.z=z;
}

void C3DView::CreateMatrix()
{
	CMatrix Tper(4,4);
	Tper.SetIdentity();
	Tper.SetAt(3,0,-(m_COP.x+m_VRP.x));
	Tper.SetAt(3,1,-(m_COP.y+m_VRP.y));
	Tper.SetAt(3,2,-(m_COP.z+m_VRP.z));
	
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
	CMatrix Trch(4,4);
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

	CMatrix TempMatrix(Tper*Trch);
	CMatrix vrp(1,4);
	vrp.SetAt(0,0,m_VRP.x);
	vrp.SetAt(0,1,m_VRP.y);
	vrp.SetAt(0,2,m_VRP.z);
	vrp.SetAt(0,3,1);
	vrp=vrp*TempMatrix;
	float a2,b2;
	CMatrix Tsh(4,4);
	Tsh.SetIdentity();
	if(vrp.GetAt(0,2) > 0.001 || vrp.GetAt(0,2)< -0.001)
	{
		a2=-((vrp.GetAt(0,0)+(m_Umax+m_Umin)/2)/vrp.GetAt(0,2));
		b2=-((vrp.GetAt(0,1)+(m_Vmax+m_Vmin)/2)/vrp.GetAt(0,2));	
		Tsh.SetAt(2,0,a2);
		Tsh.SetAt(2,1,b2);
	}

	CMatrix Tsper(4,4);
	CMatrix Mper(4,4);
	float sx,sy,sz,d;
	sz=1/(vrp.GetAt(0,2)+m_B);
	sx=2*vrp.GetAt(0,2)/(m_Umax-m_Umin) * sz;
	sy=2*vrp.GetAt(0,2)/(m_Vmax-m_Vmin) * sz;
	d=1/vrp.GetAt(0,2);
	Tsper.SetAt(0,0,sx);
	Tsper.SetAt(1,1,sy);
	Tsper.SetAt(2,2,sz);
	Tsper.SetAt(3,3,1);
	Mper.SetIdentity();
	Mper.SetAt(2,3,d);
	Mper.SetAt(3,3,0);
	m_Trans=TempMatrix * Tsh  * Mper;
}

/////////////////////////////////////////////////////////////////////////////
// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CScrollView::AssertValid();
}

void C3DView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C3DView message handlers

int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CPaintDC dc(this);
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,Cx,Cy);
	m_MemDC.SelectObject(&m_Bitmap);


	m_MemDC.SetViewportExt(Cx,Cy);

	m_MemDC.SetViewportOrg(Cx/2,Cy/2);
	Clear(RGB(0,0,0));
	Draw();
	m_3Ddlg.Create(IDD_3DSETUP);
	m_3Ddlg.SetView(this);
	m_3Ddlg.ShowWindow(SW_SHOW);
	m_3Ddlg.MoveWindow(CRect(50,200,350,700));
	return 0;
}

void C3DView::OnSaveBmp() 
{
	// TODO: Add your command handler code here
    CString lpszPathName;
    char szFilter[]="Bmp Files (*.Bmp)|*.Bmp||";
    CFileDialog FileDlg(FALSE,".Bmp",
        "MyStock",OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,
        szFilter);
    if(FileDlg.DoModal()!=IDOK)
        return;

    lpszPathName=FileDlg.GetPathName();
    CCaptureBitmap bitmap;
	bitmap.Capture(CRect(-Cx/2,-Cy/2,Cx,Cy),m_MemDC);
    bitmap.SaveBitmapEx(lpszPathName);

}

void C3DView::OnShowSetup() 
{
	// TODO: Add your command handler code here
	m_3Ddlg.DestroyWindow();
	m_3Ddlg.Create(IDD_3DSETUP);
	m_3Ddlg.SetView(this);
	m_3Ddlg.ShowWindow(SW_SHOW);
	m_3Ddlg.MoveWindow(CRect(50,200,350,700));
	
}

void C3DView::OnUpdateShowSetup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_3Ddlg.IsWindowVisible());
}
