// ChildView.cpp : implementation of the CChildView class
//
#include "stdafx.h"
#include <math.h>
#include "FloatLevel.h"
#include "ChildView.h"
#define PI  3.1415925

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
typedef CMatrixT<double> CMatrix;

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
/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CHAR()
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
	FloatLevel(dc,-2*PI,2*PI,-2*PI,2*PI,f,t);
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
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


void CChildView::FloatLevel(CDC& dc,double Xmin,double Xmax,double Zmin,double Zmax,double(*f)(double,double),void(*p)(double&,double&,double))
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

void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)	
	{
	case 'b':	xdelta+=5;
				break;
	}
	Invalidate();
}
