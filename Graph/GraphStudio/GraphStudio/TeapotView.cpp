// TeapotView.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "TeapotView.h"
#include "CaptureBitmap.h"
#include "CDib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PI  3.1415926

int CTeapotView::Cx;
int CTeapotView::Cy;
	float xDelta;
	float yDelta;
	int count;

/////////////////////////////////////////////////////////////////////////////
// CTeapotView
IMPLEMENT_DYNCREATE(CTeapotView,CScrollView)
CTeapotView::CTeapotView()
{
	count=0;
	xDelta=10.0;
	yDelta=15.0;

HuTi[0].x=1.4;
HuTi[0].y=2.25;
HuTi[0].z=0;
HuTi[1].x=1.3375;
HuTi[1].y=2.38125;
HuTi[1].z=0;
HuTi[2].x=1.4375;
HuTi[2].y=2.38125;
HuTi[2].z=0;
HuTi[3].x=1.5;
HuTi[3].y=2.25;
HuTi[3].z=0;
HuTi[4].x=1.75;
HuTi[4].y=1.725;
HuTi[4].z=0;
HuTi[5].x=2.0;
HuTi[5].y=1.2;
HuTi[5].z=0;
HuTi[6].x=2.0;
HuTi[6].y=0.75;
HuTi[6].z=0;
HuTi[7].x=2.0;
HuTi[7].y=0.3;
HuTi[7].z=0;
HuTi[8].x=1.5;
HuTi[8].y=0.075;
HuTi[8].z=0;
HuTi[9].x=1.5;
HuTi[9].y=0;
HuTi[9].z=0;

HuTi[10].x=1.5;
HuTi[10].y=-0.075;
HuTi[10].z=0;
HuTi[11].x=0.75;
HuTi[11].y=0.075;
HuTi[11].z=0;
HuTi[12].x=0;
HuTi[12].y=0.075;
HuTi[12].z=0;


HuGai[0].x=0;
HuGai[0].y=3.0;
HuGai[0].z=0;
HuGai[1].x=0.8;
HuGai[1].y=3.0;
HuGai[1].z=0;
HuGai[2].x=0;
HuGai[2].y=2.7;
HuGai[2].z=0;
HuGai[3].x=0.2;
HuGai[3].y=2.55;
HuGai[3].z=0;
HuGai[4].x=0.4;
HuGai[4].y=2.40;
HuGai[4].z=0;
HuGai[5].x=1.3;
HuGai[5].y=2.40;
HuGai[5].z=0;
HuGai[6].x=1.3;
HuGai[6].y=2.25;
HuGai[6].z=0;

HuZui[0].x=1.7;
HuZui[0].y=1.27;
HuZui[0].z=0;
HuZui[1].x=1.7;
HuZui[1].y=1.27;
HuZui[1].z=-0.66;
HuZui[2].x=1.7;
HuZui[2].y=0.4;
HuZui[2].z=-0.66;
HuZui[3].x=1.7;
HuZui[3].y=0.4;
HuZui[3].z=0;
HuZui[4].x=2.6;
HuZui[4].y=1.275;
HuZui[4].z=0;
HuZui[5].x=2.6;
HuZui[5].y=1.275;
HuZui[5].z=-0.66;
HuZui[6].x=3.1;
HuZui[6].y=0.675;
HuZui[6].z=-0.66;
HuZui[7].x=3.1;
HuZui[7].y=0.675;
HuZui[7].z=0;
HuZui[8].x=2.3;
HuZui[8].y=1.95;
HuZui[8].z=0;
HuZui[9].x=2.3;
HuZui[9].y=1.95;
HuZui[9].z=-0.25;
HuZui[10].x=2.4;
HuZui[10].y=1.875;
HuZui[10].z=-0.25;
HuZui[11].x=2.4;
HuZui[11].y=1.875;
HuZui[11].z=0;
HuZui[12].x=2.7;
HuZui[12].y=2.25;
HuZui[12].z=0;
HuZui[13].x=2.7;
HuZui[13].y=2.25;
HuZui[13].z=-0.25;
HuZui[14].x=3.3;
HuZui[14].y=2.2;
HuZui[14].z=-0.25;
HuZui[15].x=3.3;
HuZui[15].y=2.2;
HuZui[15].z=0;
HuZui[16].x=2.7;
HuZui[16].y=2.25;
HuZui[16].z=0;
HuZui[17].x=2.7;
HuZui[17].y=2.25;
HuZui[17].z=-0.25;
HuZui[18].x=3.3;
HuZui[18].y=2.2;
HuZui[18].z=-0.25;
HuZui[19].x=3.3;
HuZui[19].y=2.2;
HuZui[19].z=0;
HuZui[20].x=2.8;
HuZui[20].y=2.325;
HuZui[20].z=0;
HuZui[21].x=2.8;
HuZui[21].y=2.325;
HuZui[21].z=-0.25;
HuZui[22].x=3.525;
HuZui[22].y=2.34375;
HuZui[22].z=-0.25;
HuZui[23].x=3.525;
HuZui[23].y=2.34375;
HuZui[23].z=0;
HuZui[24].x=2.9;
HuZui[24].y=2.325;
HuZui[24].z=0;
HuZui[25].x=2.9;
HuZui[25].y=2.325;
HuZui[25].z=-0.15;
HuZui[26].x=3.4;
HuZui[26].y=2.3625;
HuZui[26].z=-0.15;
HuZui[27].x=3.4;
HuZui[27].y=2.3625;
HuZui[27].z=0;
HuZui[28].x=2.8;
HuZui[28].y=2.25;
HuZui[28].z=0;
HuZui[29].x=2.8;
HuZui[29].y=2.25;
HuZui[29].z=-0.15;
HuZui[30].x=3.2;
HuZui[30].y=2.25;
HuZui[30].z=-0.15;
HuZui[31].x=3.2;
HuZui[31].y=2.25;
HuZui[31].z=0;


HuZui2[0].x=1.7;
HuZui2[0].y=1.27;
HuZui2[0].z=0;
HuZui2[1].x=1.7;
HuZui2[1].y=1.27;
HuZui2[1].z=0.66;
HuZui2[2].x=1.7;
HuZui2[2].y=0.4;
HuZui2[2].z=0.66;
HuZui2[3].x=1.7;
HuZui2[3].y=0.4;
HuZui2[3].z=0;
HuZui2[4].x=2.6;
HuZui2[4].y=1.275;
HuZui2[4].z=0;
HuZui2[5].x=2.6;
HuZui2[5].y=1.275;
HuZui2[5].z=0.66;
HuZui2[6].x=3.1;
HuZui2[6].y=0.675;
HuZui2[6].z=0.66;
HuZui2[7].x=3.1;
HuZui2[7].y=0.675;
HuZui2[7].z=0;
HuZui2[8].x=2.3;
HuZui2[8].y=1.95;
HuZui2[8].z=0;
HuZui2[9].x=2.3;
HuZui2[9].y=1.95;
HuZui2[9].z=0.25;
HuZui2[10].x=2.4;
HuZui2[10].y=1.875;
HuZui2[10].z=0.25;
HuZui2[11].x=2.4;
HuZui2[11].y=1.875;
HuZui2[11].z=0;
HuZui2[12].x=2.7;
HuZui2[12].y=2.25;
HuZui2[12].z=0;
HuZui2[13].x=2.7;
HuZui2[13].y=2.25;
HuZui2[13].z=0.25;
HuZui2[14].x=3.3;
HuZui2[14].y=2.2;
HuZui2[14].z=0.25;
HuZui2[15].x=3.3;
HuZui2[15].y=2.2;
HuZui2[15].z=0;
HuZui2[16].x=2.7;
HuZui2[16].y=2.25;
HuZui2[16].z=0;
HuZui2[17].x=2.7;
HuZui2[17].y=2.25;
HuZui2[17].z=0.25;
HuZui2[18].x=3.3;
HuZui2[18].y=2.2;
HuZui2[18].z=0.25;
HuZui2[19].x=3.3;
HuZui2[19].y=2.2;
HuZui2[19].z=0;
HuZui2[20].x=2.8;
HuZui2[20].y=2.325;
HuZui2[20].z=0;
HuZui2[21].x=2.8;
HuZui2[21].y=2.325;
HuZui2[21].z=0.25;
HuZui2[22].x=3.525;
HuZui2[22].y=2.34375;
HuZui2[22].z=0.25;
HuZui2[23].x=3.525;
HuZui2[23].y=2.34375;
HuZui2[23].z=0;
HuZui2[24].x=2.9;
HuZui2[24].y=2.325;
HuZui2[24].z=0;
HuZui2[25].x=2.9;
HuZui2[25].y=2.325;
HuZui2[25].z=0.15;
HuZui2[26].x=3.4;
HuZui2[26].y=2.3625;
HuZui2[26].z=0.15;
HuZui2[27].x=3.4;
HuZui2[27].y=2.3625;
HuZui2[27].z=0;
HuZui2[28].x=2.8;
HuZui2[28].y=2.25;
HuZui2[28].z=0;
HuZui2[29].x=2.8;
HuZui2[29].y=2.25;
HuZui2[29].z=0.15;
HuZui2[30].x=3.2;
HuZui2[30].y=2.25;
HuZui2[30].z=0.15;
HuZui2[31].x=3.2;
HuZui2[31].y=2.25;
HuZui2[31].z=0;

BaShou[0].x=-1.6;
BaShou[0].y=1.875;
BaShou[0].z=0;
BaShou[1].x=-1.6;
BaShou[1].y=1.875;
BaShou[1].z=-0.3;
BaShou[2].x=-1.5;
BaShou[2].y=2.1;
BaShou[2].z=-0.3;
BaShou[3].x=-1.5;
BaShou[3].y=2.1;
BaShou[3].z=0;
BaShou[4].x=-2.3;
BaShou[4].y=1.875;
BaShou[4].z=0;
BaShou[5].x=-2.3;
BaShou[5].y=1.875;
BaShou[5].z=-0.3;
BaShou[6].x=-2.5;
BaShou[6].y=2.1;
BaShou[6].z=-0.3;
BaShou[7].x=-2.5;
BaShou[7].y=2.1;
BaShou[7].z=0;
BaShou[8].x=-2.7;
BaShou[8].y=1.875;
BaShou[8].z=0;
BaShou[9].x=-2.7;
BaShou[9].y=1.875;
BaShou[9].z=-0.3;
BaShou[10].x=-3.0;
BaShou[10].y=2.1;
BaShou[10].z=-0.3;
BaShou[11].x=-3.0;
BaShou[11].y=2.1;
BaShou[11].z=0;
BaShou[12].x=-2.7;
BaShou[12].y=1.65;
BaShou[12].z=0;
BaShou[13].x=-2.7;
BaShou[13].y=1.65;
BaShou[13].z=-0.3;
BaShou[14].x=-3.0;
BaShou[14].y=1.65;
BaShou[14].z=-0.3;
BaShou[15].x=-3.0;
BaShou[15].y=1.65;
BaShou[15].z=0;
BaShou[16].x=-2.7;
BaShou[16].y=1.65;
BaShou[16].z=0;
BaShou[17].x=-2.7;
BaShou[17].y=1.65;
BaShou[17].z=-0.3;
BaShou[18].x=-3.0;
BaShou[18].y=1.65;
BaShou[18].z=-0.3;
BaShou[19].x=-3.0;
BaShou[19].y=1.65;
BaShou[19].z=0;
BaShou[20].x=-2.7;
BaShou[20].y=1.425;
BaShou[20].z=0;
BaShou[21].x=-2.7;
BaShou[21].y=1.425;
BaShou[21].z=-0.3;
BaShou[22].x=-3.0;
BaShou[22].y=1.20;
BaShou[22].z=-0.3;
BaShou[23].x=-3.0;
BaShou[23].y=1.20;
BaShou[23].z=0;
BaShou[24].x=-2.5;
BaShou[24].y=0.975;
BaShou[24].z=0;
BaShou[25].x=-2.5;
BaShou[25].y=0.975;
BaShou[25].z=-0.3;
BaShou[26].x=-2.65;
BaShou[26].y=0.7875;
BaShou[26].z=-0.3;
BaShou[27].x=-2.65;
BaShou[27].y=0.7875;
BaShou[27].z=0;
BaShou[28].x=-2.0;
BaShou[28].y=0.75;
BaShou[28].z=0;
BaShou[29].x=-2.0;
BaShou[29].y=0.75;
BaShou[29].z=-0.3;
BaShou[30].x=-1.90;
BaShou[30].y=0.45;
BaShou[30].z=-0.3;
BaShou[31].x=-1.90;
BaShou[31].y=0.45;
BaShou[31].z=0;

BaShou2[0].x=-1.6;
BaShou2[0].y=1.875;
BaShou2[0].z=0;
BaShou2[1].x=-1.6;
BaShou2[1].y=1.875;
BaShou2[1].z=0.3;
BaShou2[2].x=-1.5;
BaShou2[2].y=2.1;
BaShou2[2].z=0.3;
BaShou2[3].x=-1.5;
BaShou2[3].y=2.1;
BaShou2[3].z=0;
BaShou2[4].x=-2.3;
BaShou2[4].y=1.875;
BaShou2[4].z=0;
BaShou2[5].x=-2.3;
BaShou2[5].y=1.875;
BaShou2[5].z=0.3;
BaShou2[6].x=-2.5;
BaShou2[6].y=2.1;
BaShou2[6].z=0.3;
BaShou2[7].x=-2.5;
BaShou2[7].y=2.1;
BaShou2[7].z=0;
BaShou2[8].x=-2.7;
BaShou2[8].y=1.875;
BaShou2[8].z=0;
BaShou2[9].x=-2.7;
BaShou2[9].y=1.875;
BaShou2[9].z=0.3;
BaShou2[10].x=-3.0;
BaShou2[10].y=2.1;
BaShou2[10].z=0.3;
BaShou2[11].x=-3.0;
BaShou2[11].y=2.1;
BaShou2[11].z=0;
BaShou2[12].x=-2.7;
BaShou2[12].y=1.65;
BaShou2[12].z=0;
BaShou2[13].x=-2.7;
BaShou2[13].y=1.65;
BaShou2[13].z=0.3;
BaShou2[14].x=-3.0;
BaShou2[14].y=1.65;
BaShou2[14].z=0.3;
BaShou2[15].x=-3.0;
BaShou2[15].y=1.65;
BaShou2[15].z=0;
BaShou2[16].x=-2.7;
BaShou2[16].y=1.65;
BaShou2[16].z=0;
BaShou2[17].x=-2.7;
BaShou2[17].y=1.65;
BaShou2[17].z=0.3;
BaShou2[18].x=-3.0;
BaShou2[18].y=1.65;
BaShou2[18].z=0.3;
BaShou2[19].x=-3.0;
BaShou2[19].y=1.65;
BaShou2[19].z=0;
BaShou2[20].x=-2.7;
BaShou2[20].y=1.425;
BaShou2[20].z=0;
BaShou2[21].x=-2.7;
BaShou2[21].y=1.425;
BaShou2[21].z=0.3;
BaShou2[22].x=-3.0;
BaShou2[22].y=1.20;
BaShou2[22].z=0.3;
BaShou2[23].x=-3.0;
BaShou2[23].y=1.20;
BaShou2[23].z=0;
BaShou2[24].x=-2.5;
BaShou2[24].y=0.975;
BaShou2[24].z=0;
BaShou2[25].x=-2.5;
BaShou2[25].y=0.975;
BaShou2[25].z=0.3;
BaShou2[26].x=-2.65;
BaShou2[26].y=0.7875;
BaShou2[26].z=0.3;
BaShou2[27].x=-2.65;
BaShou2[27].y=0.7875;
BaShou2[27].z=0;
BaShou2[28].x=-2.0;
BaShou2[28].y=0.75;
BaShou2[28].z=0;
BaShou2[29].x=-2.0;
BaShou2[29].y=0.75;
BaShou2[29].z=0.3;
BaShou2[30].x=-1.90;
BaShou2[30].y=0.45;
BaShou2[30].z=0.3;
BaShou2[31].x=-1.90;
BaShou2[31].y=0.45;
BaShou2[31].z=0;
}

CTeapotView::~CTeapotView()
{
}

void Proc(CDC& dc,Point3D& p)
{
	CMatrix temp(1,4);
	temp.SetAt(0,0,p.x);
	temp.SetAt(0,1,p.y);
	temp.SetAt(0,2,p.z);
	temp.SetAt(0,3,1);
	CMatrix tran(4,4),tran2(4,4);
	float t1=cos(yDelta*PI/180.0);
	float t2=sin(yDelta*PI/180.0);
	tran.SetIdentity();
	tran.SetAt(0,0,t1);
	tran.SetAt(0,2,-t2);
	tran.SetAt(2,0,t2);
	tran.SetAt(2,2,t1);
	t1=cos(xDelta*PI/180.0);
	t2=sin(xDelta*PI/180.0);
	tran2.Clear();
	tran2.SetIdentity();
	tran2.SetAt(1,1,t1);
	tran2.SetAt(1,2,t2);
	tran2.SetAt(2,1,-t2);
	tran2.SetAt(2,2,t1);

	temp=temp*tran*tran2;
	p.x=temp.GetAt(0,0);
	p.y=temp.GetAt(0,1);
	p.z=temp.GetAt(0,2);
}

void Proc2(CDC& dc,Point3D& p)
{
	int i=0, j=0;
	static Point3D back[250];
	Point3D backloc,pcur,pbak;
	Point3D pp;
	float t1,t2;
	float delta=4;
	CMatrix temp(1,4),temp2(1,4);
	CMatrix tran1(4,4),tran2(4,4);

	for(;delta<=360;delta+=4,i++)
	{
		tran1.Clear();
		temp.SetAt(0,0,p.x);
		temp.SetAt(0,1,p.y);
		temp.SetAt(0,2,p.z);
		temp.SetAt(0,3,1);
		t1=cos(delta*PI/180.0);
		t2=sin(delta*PI/180.0);
		tran1.SetIdentity();
		tran1.SetAt(0,0,t1);
		tran1.SetAt(0,2,-t2);
		tran1.SetAt(2,0,t2);
		tran1.SetAt(2,2,t1);
//		temp=temp*tran;
		t1=cos(xDelta*PI/180.0);
		t2=sin(xDelta*PI/180.0);
		tran2.Clear();
		tran2.SetIdentity();
		tran2.SetAt(1,1,t1);
		tran2.SetAt(1,2,t2);
		tran2.SetAt(2,1,-t2);
		tran2.SetAt(2,2,t1);
		temp=temp*tran1*tran2;
		pcur.x=temp.GetAt(0,0);
		pcur.y=temp.GetAt(0,1);
		pcur.z=temp.GetAt(0,2);
		if( i>=0&&i%2==0 &&count==0)
		{
			back[j]=pcur;
			j++;
		}
		if(i==0)
		{
			pbak=pcur;
			backloc=pcur;
		}
		else
		{
			CTeapotView::Line(dc,pcur,backloc);
			backloc=pcur;
		}
		if( i>=0&&i%2==0 && count >0)
		{
			CTeapotView::Line(dc,pcur,back[j]);
			back[j]=pcur;
			j++;
		}
	}
	CTeapotView::Line(dc,pcur,pbak);
	count++;
}

BEGIN_MESSAGE_MAP(CTeapotView, CScrollView)
	//{{AFX_MSG_MAP(CTeapotView)
	ON_WM_CREATE()
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_COMMAND(ID_TEAPOT_X, OnTeapotX)
	ON_COMMAND(ID_TEAPOT_Y, OnTeapotY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CTeapotView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CPaintDC dc(this);
	Cx=1024;
	Cy=768;
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,Cx,Cy);
	m_MemDC.SelectObject(&m_Bitmap);
	Draw();	

	
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CTeapotView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CScrollView::PreCreateWindow(cs))
		return FALSE;
/*
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);
*/
	return TRUE;
}

void CTeapotView::Line(CDC& dc,Point3D& x,Point3D& y)
{
	dc.MoveTo((int)(Cx/2-x.x*100),(int)(Cy/2-x.y*100));
	dc.LineTo((int)(Cx/2-y.x*100),(int)(Cy/2-y.y*100));
}

void CTeapotView::Bazel(CDC& dc,Point3D* point,int size,int n,void(*p)(CDC&,Point3D&))
{
	if(size!=4)
		TRACE("Warning: not supported\n");
	float temp;
	float u=1.0/n;
	float x,y,z;
	float u0,u1,u2,u3;
	Point3D ptemp(point[0]);
	Point3D pbk=ptemp;
	for(temp=u;temp<=1.0;temp+=u)
	{
		u0=-temp*temp*temp+3*temp*temp-3*temp+1;
		u1=3*temp*temp*temp-6*temp*temp+3*temp;
		u2=-3*temp*temp*temp+3*temp*temp;
		u3=temp*temp*temp;
		x=u0*point[0].x+u1*point[1].x+u2*point[2].x+u3*point[3].x;
		y=u0*point[0].y+u1*point[1].y+u2*point[2].y+u3*point[3].y;
		z=u0*point[0].z+u1*point[1].z+u2*point[2].z+u3*point[3].z;
		pbk=Point3D(x,y,z);
		if(p!=NULL)
			(*p)(dc,pbk);
	}
}

void CTeapotView::BazelEx(CDC& dc,Point3D* point,int size,int n,void(*p)(CDC&,Point3D&))
{
	int x,y,i;
	if(size!=16)
		TRACE("Warning: not supported\n");
	CMatrix Vx(4,4),Vy(4,4),Vz(4,4);
	i=0;
	for(x=0;x<4;x++)
	{
		for(y=0;y<4;y++)
		{
			Vx.SetAt(x,y,point[i].x);
			Vy.SetAt(x,y,point[i].y);
			Vz.SetAt(x,y,point[i].z);
			i++;
		}
	}
	CMatrix B(4,4);
	B.SetAt(0,0,-1);
	B.SetAt(0,1,3);
	B.SetAt(0,2,-3);
	B.SetAt(0,3,1);
	B.SetAt(1,0,3);
	B.SetAt(1,1,-6);
	B.SetAt(1,2,3);
	B.SetAt(1,3,0);
	B.SetAt(2,0,-3);
	B.SetAt(2,1,3);
	B.SetAt(2,2,0);
	B.SetAt(2,3,0);
	B.SetAt(3,0,1);
	B.SetAt(3,1,0);
	B.SetAt(3,2,0);
	B.SetAt(3,3,0);
//	B.Transform(BT);
	float temp=1.0/n;
	CMatrix U(1,4),W(4,1);
	Point3D pp[20];
	Point3D pbak;
	Point3D pbak2(point[0]);
	if(p!=NULL)
		(*p)(dc,pbak2);
	Point3D pt(pbak2),ptb(pbak2);
	U.SetAt(0,3,1);		W.SetAt(3,0,1);		W.SetAt(1,0,0);		W.SetAt(2,0,0);		W.SetAt(0,0,0);
	float u,w;
	float xt,yt,zt;
	for(u=temp,i=0;u<=1.0;u+=temp,i++)
	{
			U.SetAt(0,0,u*u*u);
			U.SetAt(0,1,u*u);
			U.SetAt(0,2,u);
			pp[i].x=(U*B*Vx*B*W).GetAt(0,0);
			pp[i].y=(U*B*Vy*B*W).GetAt(0,0);
			pp[i].z=(U*B*Vz*B*W).GetAt(0,0);
			if(p!=NULL)
				(*p)(dc,pp[i]);
			if(i==0)
				Line(dc,pt,pp[0]);
			else
				Line(dc,pp[i-1],pp[i]);
	}
	for(w=temp;w<=1;w+=temp)
	{
		for(i=-1,u=0;u<=1.0;u+=temp,i++)
		{
			U.SetAt(0,0,u*u*u);
			U.SetAt(0,1,u*u);
			U.SetAt(0,2,u);
			W.SetAt(0,0,w*w*w);
			W.SetAt(1,0,w*w);
			W.SetAt(2,0,w);
			xt=(U*B*Vx*B*W).GetAt(0,0);
			yt=(U*B*Vy*B*W).GetAt(0,0);
			zt=(U*B*Vz*B*W).GetAt(0,0);
			pbak=Point3D(xt,yt,zt);
			if(p!=NULL){
				(*p)(dc,pbak);
			}
			if(i==-1)
			{
				Line(dc,ptb,pbak);
				ptb=pbak;
				pt=ptb;
			}
			else
			{
				Line(dc,pp[i],pbak);
				pp[i]=pbak;
				Line(dc,pt,pp[i]);
				pt=pp[i];
			}
		}
	}
}

void CTeapotView::Clear(COLORREF clr)
{
	CBrush brush(clr);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,Cx,Cy,PATCOPY);

}

void CTeapotView::Draw()
{
	count=0;
	Clear(RGB(255,255,255));
	Bazel(m_MemDC,HuGai,4,9,Proc2);
	Bazel(m_MemDC,HuGai+3,4,9,Proc2);
	Bazel(m_MemDC,HuTi,4,9,Proc2);
	Bazel(m_MemDC,HuTi+3,4,9,Proc2);
	Bazel(m_MemDC,HuTi+6,4,9,Proc2);
	Bazel(m_MemDC,HuTi+9,4,9,Proc2);
	
	BazelEx(m_MemDC,HuZui,16,8,Proc);
	BazelEx(m_MemDC,HuZui2,16,8,Proc);
	BazelEx(m_MemDC,HuZui+16,16,8,Proc);
	BazelEx(m_MemDC,HuZui2+16,16,8,Proc);
	BazelEx(m_MemDC,BaShou,16,8,Proc);
	BazelEx(m_MemDC,BaShou2,16,8,Proc);
	BazelEx(m_MemDC,BaShou+16,16,8,Proc);
	BazelEx(m_MemDC,BaShou2+16,16,8,Proc);
}

void CTeapotView::SaveToBitmap(CString& szPath,CRect Rect)
{

    CCaptureBitmap bitmap;
	bitmap.Capture(Rect,m_MemDC);
    bitmap.SaveBitmapEx(szPath);
}

void CTeapotView::OnSaveBmp() 
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
	CRect Rect;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);
	SaveToBitmap(lpszPathName,CRect(0,0,1024,768));
}

void CTeapotView::OnDraw(CDC* pDc)
{	
	pDc->BitBlt(0,0,Cx,Cy,&m_MemDC,0,0,SRCCOPY);
}

void CTeapotView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CRect rc;
	CSize sizeTotal;
	sizeTotal.cx = 1024; // size required to display image
	sizeTotal.cy = 768; // size required to display image
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void CTeapotView::OnTeapotX() 
{
	// TODO: Add your command handler code here
	xDelta+=10.0;
	Draw();
	Invalidate(FALSE);
	
}

void CTeapotView::OnTeapotY() 
{
	// TODO: Add your command handler code here
	yDelta+=10.0;
	Draw();
	Invalidate(FALSE);
}
