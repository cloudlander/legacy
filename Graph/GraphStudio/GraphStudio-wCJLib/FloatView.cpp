// FloatView.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "FloatView.h"
#include "CaptureBitmap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PI  3.1415925

/////////////////////////////////////////////////////////////////////////////
// CFloatView
float xdelta=25,ydelta=15;
double f(double x,double z)
{
	double R;
	R=sqrt(x*x+z*z);
	return (3*cos(1.2*R))/(R+1);
}

void t(double& x,double& y,double z)
{
	CMatrix temp(1,4);
	temp.SetAt(0,0,x);
	temp.SetAt(0,1,y);
	temp.SetAt(0,2,z);
	temp.SetAt(0,3,1);
	CMatrix tran(4,4),tran2(4,4);
	float t1=cos(ydelta*PI/180.0);
	float t2=sin(ydelta*PI/180.0);
	tran.SetIdentity();
	tran.SetAt(0,0,t1);
	tran.SetAt(0,2,-t2);
	tran.SetAt(2,0,t2);
	tran.SetAt(2,2,t1);
	t1=cos(xdelta*PI/180.0);
	t2=sin(xdelta*PI/180.0);
	tran2.Clear();
	tran2.SetIdentity();
	tran2.SetAt(1,1,t1);
	tran2.SetAt(1,2,t2);
	tran2.SetAt(2,1,-t2);
	tran2.SetAt(2,2,t1);
	temp=temp*tran*tran2;
	x=temp.GetAt(0,0);
	y=temp.GetAt(0,1);
	x=400+50*x;
	y=300-50*y;
}

int Sign(double x)
{
	if(x>=0.0)
		return 1;
	else
		return 0;
}

double Max(double x1,double x2)
{
	if(x1 > x2)
		return x1;
	else
		return x2;
}
double Min(double x1,double x2)
{
	if(x1 > x2)
		return x2;
	else
		return x1;
}
IMPLEMENT_DYNCREATE(CFloatView, CScrollView)

CFloatView::CFloatView()
{
}

CFloatView::~CFloatView()
{
}


BEGIN_MESSAGE_MAP(CFloatView, CScrollView)
	//{{AFX_MSG_MAP(CFloatView)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatView drawing

void CFloatView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 1024;
	sizeTotal.cy = 768;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CFloatView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	pDC->BitBlt(0,0,Cx,Cy,&m_MemDC,0,0,SRCCOPY);
}

void Line(CDC& dc,double xp,double yp,double x,double y)
{
	dc.MoveTo((int)xp,(int)yp);
	dc.LineTo((int)x,(int)y);
}

void Visibility(double x,double y,double* u,double* l,int& f)
{
	if(y<u[(int)x] && y>l[(int)x])
		f=0;
	else
	{
		if(y>=u[(int)x])
			f=1;
		else
			f=-1;
	}
}

void Intersect(double x1,double y1,double x2,double y2,double* array,double& xx,double& yy)
{
	int xinc=Sign(x2-x1);
	if(xinc==0)
	{
		xx=x2;	
		yy=array[(int)x2];
	}
	else
	{
		double s=(y2-y1)/(x2-x1);
		double ys=Sign(y1-array[(int)x1]);
		yy=y1+s;
		for(xx=x1+xinc;xx<=x2;xx+=xinc)
		{
			if(ys!=Sign(yy-array[(int)xx]))
				return;
			yy+=s;
		}
	}
}

void Horizon(double x1,double y1,double x2,double y2,double* u,double* l)
{
	int xinc=Sign(x2-x1);
	double y;
	if(xinc==0)
	{
		u[(int)x2]=Max(u[(int)x2],y2);
		l[(int)x2]=Min(l[(int)x2],y2);
	}
	else
	{
		double s=(y2-y1)/(x2-x1);
		for(double x=x1;x<=x2;x+=xinc)
		{
			y=s*(x-x1)+y1;
			u[(int)x]=Max(u[(int)x],y);
			l[(int)x]=Min(l[(int)x],y);
		}
	}
}

void Efill(double x,double y,double& xedge,double& yedge,double* u,double* l)
{
	if(xedge==-1){
		xedge=x;yedge=y;
		return;
	}
	else
		Horizon(xedge,yedge,x,y,u,l);

}


void CFloatView::FloatLevel(CDC& dc,double Xmin,double Xmax,double Zmin,double Zmax,double(*f)(double,double),void(*p)(double&,double&,double))
{
	double upper[15000],lower[15000];
	int pflag=-2,cflag=-2;
	double xleft=-1;
	double yleft=-1;
	double xright=-1;
	double yright=-1;
	int i;
	for(i=0;i<1000;i++)
	{
		upper[i]=0.0;
		lower[i]=600.0;
	}
	double z,x,y,xt;
	double xp,yp;
	double xx=-1000,yy=-1000,xu=-1000,yu=-1000,xl=-1000,yl=-1000;
	for(z=Zmax;z>=Zmin;z=z-0.2)
	{
		xp=Xmin;	yp=(*f)(xp,z);
		(*p)(xp,yp,z);					// transform
		Efill(xp,yp,xleft,yleft,upper,lower);
		Visibility(xp,yp,upper,lower,pflag);
		for(xt=Xmin;xt<=Xmax;xt+=0.2)
		{
			x=xt;	
			y=(*f)(x,z);
			(*p)(x,y,z);
			Visibility(x,y,upper,lower,cflag);

			if(cflag==pflag)
			{
				if(cflag==1 || cflag==-1)
				{
					Line(dc,xp,yp,x,y);
					Horizon(xp,yp,x,y,upper,lower);
				}
			}
			else
			{
				if(cflag==0)
				{
					if(pflag==1)
						Intersect(xp,yp,x,y,upper,xx,yy);
					else
						Intersect(xp,yp,x,y,lower,xx,yy);
					Line(dc,xp,yp,xx,yy);
					Horizon(xp,yp,xx,yy,upper,lower);
				}
				else
				{
					if(cflag==1)
					{
						if(pflag==0)
						{
							Intersect(xp,yp,x,y,upper,xx,yy);
							Line(dc,xx,yy,x,y);
							Horizon(xx,yy,x,y,upper,lower);
						}
						else
						{
							Intersect(xp,yp,x,y,lower,xx,yy);
							Line(dc,xp,yp,xx,yy);
							Horizon(xp,yp,xx,yy,upper,lower);
							Intersect(xp,yp,x,y,upper,xu,yu);
							Line(dc,xu,yu,x,y);
							Horizon(xu,yu,x,y,upper,lower);
						}
					}
					else
					{
						if(pflag==0)
						{
							Intersect(xp,yp,x,y,lower,xl,yl);
							Line(dc,xl,yl,x,y);
							Horizon(xl,yl,x,y,upper,lower);
						}
						else
						{
							Intersect(xp,yp,x,y,upper,xu,yu);
							Line(dc,xp,yp,xu,yu);
							Horizon(xp,yp,xu,yu,upper,lower);
							Intersect(xp,yp,x,y,lower,xl,yl);
							Line(dc,xl,yl,x,y);
							Horizon(xl,yl,x,y,upper,lower);
						}
					}
				}
			}
			pflag=cflag;
			xp=x;
			yp=y;
		}
		Efill(x,y,xright,yright,upper,lower);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CFloatView diagnostics

#ifdef _DEBUG
void CFloatView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFloatView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFloatView message handlers

void CFloatView::OnProperty() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("浮动水平线算法");
}

void CFloatView::OnSaveBmp() 
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
	bitmap.Capture(CRect(0,0,1024,768),m_MemDC);
    bitmap.SaveBitmapEx(lpszPathName);
}

int CFloatView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CPaintDC dc(this);
	Cx=1024;
	Cy=768;
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,Cx,Cy);
	m_MemDC.SelectObject(&m_Bitmap);
	HBRUSH brush=(HBRUSH)::GetStockObject(WHITE_BRUSH);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,Cx,Cy,PATCOPY);
	FloatLevel(m_MemDC,-2*PI,2*PI,-2*PI,2*PI,f,t);
	return 0;
}
