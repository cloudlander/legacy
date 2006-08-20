#include <stdafx.h>
#include <math.h>
#include "Poly.h"
#include "Matrix.h"
#define PI  3.1415926
typedef CMatrixT<double> CMatrix;
	// 原始边表
	double yDelta=30;
	double xDelta=15;
Line    LTable[25]={{-1000,-1000},{1,2},{2,3},{3,4}
,{4,1},	{5,6},{6,7},{7,8},{8,5},{1,5},{2,6},
{3,7},{4,8},{9,10},{10,11},{11,12},{12,9},
{13,14},{14,15},{15,16},{16,13},{9,13},{10,14},
{11,15},{12,16}
};
int sti,cti;
Line	CTLTable[25];			
LineRaw STable[25];					// 去掉遮挡部分后新生成的边表
Point3D	CTPTable[20];				// 穿透点表
// data not inserted!!!!
Point3D PTable[17]={{-1000,-1000,-1000},{0,0,1},{2,0,1},{2,0,3},{0,0,3},{0,6,1},{2,6,1},{2,6,3},
{0,6,3},{1,2,0},{3,2,0},{3,2,4},{1,2,4},{1,4,0},{3,4,0},{3,4,4},{1,4,4}};					
PolyList g_PolyList;
CMatrix area1(4,6);
CMatrix area2(4,6);		// 形体矩阵(暂时作为单独的全局变量处理)

void Init();
void SetMinMax();
void Transform(CMatrix& mx);
void NewWell(CMatrix& mx,PolyNode& pn);
void PolyValidate(CMatrix& area,PolyNode& pn);
void HideArea(AreaList& al);
bool TestLineHide(int LineNo,PolyNode& tester);
bool CompleteTest(int p1,int p2,int index,bool bThrough);
void GetTRange(double& tmin,double& tmax,double a,double t,CMatrix& p,CMatrix& q,CMatrix& w,CMatrix& s,CMatrix& d,bool bThrough);
void InsertSTable(int p1,int p2,CMatrix& MP1,CMatrix& MP2);
void InsertCTPTable(double x,double y,double z);
void DrawLine(CDC& dc,int p1,int p2);
void Draw(CDC& dc);
void DebugMatrix(CMatrix& temp);

void Roberts()
{
	CMatrix p1(8,4),p2(8,4);
	int i=1;
		Init();
	for(;i<=8;i++)
	{
		p1.SetAt(i-1,0,PTable[i].x);
		p1.SetAt(i-1,1,PTable[i].y);
		p1.SetAt(i-1,2,PTable[i].z);
		p1.SetAt(i-1,3,1);
	}
	for(;i<=16;i++)
	{
		p2.SetAt(i-9,0,PTable[i].x);
		p2.SetAt(i-9,1,PTable[i].y);
		p2.SetAt(i-9,2,PTable[i].z);
		p2.SetAt(i-9,3,1);
	}
	Transform(p1);
	Transform(p2);
	DebugMatrix(p1);
	DebugMatrix(p2);
	for(i=1;i<=8;i++)
	{
		PTable[i].x=p1.GetAt(i-1,0);
		PTable[i].y=p1.GetAt(i-1,1);
		PTable[i].z=p1.GetAt(i-1,2);
	}
	for(;i<=16;i++)
	{
		PTable[i].x=p2.GetAt(i-9,0);
		PTable[i].y=p2.GetAt(i-9,1);
		PTable[i].z=p2.GetAt(i-9,2);
	}
	SetMinMax();
	DebugMatrix(p1);
	DebugMatrix(p2);
	// 生成形体矩阵
	PolyList::iterator pli=g_PolyList.begin();
		NewWell(area1,*pli);
		PolyValidate(area1,*pli);
	// 自隐面检查
		for(i=0;i<6;i++)
		{
			if(area1.GetAt(2,i)>0)
			{
				(*pli).al[i].bHide=true;							
			}
		}

		pli++;
		NewWell(area2,*pli);
		PolyValidate(area2,*pli);
		for(i=0;i<6;i++)
		{
			if(area2.GetAt(2,i)>0)
			{
				(*pli).al[i].bHide=true;							
			}
		}
		
	pli=g_PolyList.begin();
	HideArea(pli->al);
	pli++;
	HideArea(pli->al);

	// 尚未排序
	// 按照形体优先级测试各个形体的边
	// Line中的范围尚未填充 !!!!!!!
	// 先hardcode有序操作
	for(i=1;i<=13;i++)
	{
		if(!LTable[i].bHide&&TestLineHide(i,*pli))
			LTable[i].bHide=true;
	}
	pli--;
	for(i=13;i<=24;i++)
	{	if(!LTable[i].bHide&&TestLineHide(i,*pli))
			LTable[i].bHide=true;
	}


}

// NewWell算法求出形体矩阵
void NewWell(CMatrix& mx,PolyNode& pn)
{
	int i=0;
	int length=pn.al.size();
	int j,k;
	double a=0,b=0,c=0,d=0;
	int n;
	for(;i<length;i++)
	{
		a=b=c=d=0;
		n=pn.al[i].pl.size()-1;
//		for(j=0;j<n;j++)
//			TRACE("%d\t",pn.al[i].pl[j]);
		for(j=0;j<n;j++)
		{
			k=(j+1)%n;
//			TRACE("%d,%d\n",pn.al[i].pl[j],pn.al[i].pl[k]);
//			TRACE("%f,%f,%f\n",PTable[pn.al[i].pl[j]].x,PTable[pn.al[i].pl[j]].y,PTable[pn.al[i].pl[j]].z);
//			TRACE("%f,%f,%f\n",PTable[pn.al[i].pl[k]].x,PTable[pn.al[i].pl[k]].y,PTable[pn.al[i].pl[k]].z);
//			TRACE("%f\t",(PTable[pn.al[i].pl[j]].y-PTable[pn.al[i].pl[k]].y)*(PTable[pn.al[i].pl[j]].z+PTable[pn.al[i].pl[k]].z));
			a=a+(PTable[pn.al[i].pl[j]].y-PTable[pn.al[i].pl[k]].y)*(PTable[pn.al[i].pl[j]].z+PTable[pn.al[i].pl[k]].z);
			b=b+(PTable[pn.al[i].pl[j]].z-PTable[pn.al[i].pl[k]].z)*(PTable[pn.al[i].pl[j]].x+PTable[pn.al[i].pl[k]].x);
			c=c+(PTable[pn.al[i].pl[j]].x-PTable[pn.al[i].pl[k]].x)*(PTable[pn.al[i].pl[j]].y+PTable[pn.al[i].pl[k]].y);
		}
		d=-(a*PTable[pn.al[i].pl[0]].x+b*PTable[pn.al[i].pl[0]].y+c*PTable[pn.al[i].pl[0]].z);
		mx.SetAt(0,i,a);
		mx.SetAt(1,i,b);
		mx.SetAt(2,i,c);
		mx.SetAt(3,i,d);
//		DebugMatrix(mx);
	}
}

// 检验是否为有效形体矩阵,不是的话,将第三行元素取反
void PolyValidate(CMatrix& area,PolyNode& pn)
{
	double x=0,y=0,z=0;
	int n=0;
	int count=0;
	int i,j;
	int length=pn.al.size();
/*
	for(i=0;i<length;i++)
	{
		n=pn.al[i].pl.size();
		for(j=0;j<n;j++)
		{
			x=x+PTable[pn.al[i].pl[j]].x;
			y=y+PTable[pn.al[i].pl[j]].y;
			z=z+PTable[pn.al[i].pl[j]].z;
			count++;
		}
	}
	x=x/(double)count;
	y=y/(double)count;
	z=z/(double)count;
*/
	CMatrix temp(1,4),temp2(1,6);
	temp.SetAt(0,0,0.299);		temp.SetAt(0,1,2.257);		temp.SetAt(0,2,3.176);		temp.SetAt(0,3,1);
	temp2=temp*area;
	for(i=0;i<6;i++)
	{
		if(temp2.GetAt(0,i)<=0)
		{
			area.SetAt(0,i,-1*area.GetAt(0,i));
			area.SetAt(1,i,-1*area.GetAt(1,i));
			area.SetAt(2,i,-1*area.GetAt(2,i));
			area.SetAt(3,i,-1*area.GetAt(3,i));
		}
	}
	DebugMatrix(area);
}

// 自隐面消除,标记边列表
void HideArea(AreaList& al)
{
	int j;
	AreaList::iterator ali=al.begin();
	int n;
	int flag[30];
		for(j=0;j<30;j++)
			flag[j]=0;
	for(;ali!=al.end();ali++)
	{	
		n=ali->ll.size();
		for(j=0;j<n;j++)
		{
/*
			if(flag[ali->ll[j]]==0)
				flag[ali->ll[j]]=1;
			else if(flag[ali->ll[j]]==0)
				flag[ali->ll[j]]=1;
			else if(flag[ali->ll[j]==1)
				flag[ali->ll[j]]=2
*/			if(ali->bHide==true)
				flag[ali->ll[j]]++;
		}
	}
	for(j=0;j<30;j++)
	{
		if(flag[j]==2)
		{
			LTable[j].bHide=true;
		}
	}
}

// testee的边测试是否被形体tester遮挡,返回true说明被遮挡
bool TestLineHide(int LineNo,PolyNode& tester)
{
	bool through=false;

				// 判别l是否完全落在形体tester的外接最小长方体之外
				if(LTable[LineNo].XMax < tester.XMin 
					|| LTable[LineNo].XMin > tester.XMax
					|| LTable[LineNo].YMax < tester.YMin
					|| LTable[LineNo].YMin > tester.YMax)
				{
					return false;
				}
				else if(LTable[LineNo].ZMin > tester.ZMax ||
					    LTable[LineNo].ZMax < tester.ZMin)
				{
					return false;
				}
				else
					through=true;			// 置穿透可能标志

				// 执行完全可见性判别
	return CompleteTest(LTable[LineNo].p1,LTable[LineNo].p2,tester.index,through);
}


// 完全可见性判别
bool CompleteTest(int p1,int p2,int index,bool bThrough)
{
	double tmin=1000,tmax=-1000;
	int i,j;
	CMatrix S(1,4),d(1,4),P(1,6),Q(1,6),W(1,6),g(1,4);
	CMatrix temp(1,4),temp2(1,4);
	double a11,a12,a21,a22,c1,c2;
	double a,t;
		S.SetAt(0,0,PTable[p1].x);
		S.SetAt(0,1,PTable[p1].y);
		S.SetAt(0,2,PTable[p1].z);
		S.SetAt(0,3,1);
		d.SetAt(0,0,PTable[p2].x-PTable[p1].x);
		d.SetAt(0,1,PTable[p2].y-PTable[p1].y);
		d.SetAt(0,2,PTable[p2].z-PTable[p1].z);
		d.SetAt(0,3,0);
		g.SetAt(0,0,0);		g.SetAt(0,1,0);		g.SetAt(0,2,1);		g.SetAt(0,3,0);
	if(index == 1)
	{
		P=S*area1;
		Q=d*area1;
		W=g*area1;
	}
	else
	{
		P=S*area2;
		Q=d*area2;
		W=g*area2;
	}
		DebugMatrix(area2);
		DebugMatrix(S);	
		DebugMatrix(d);
		DebugMatrix(P);
		DebugMatrix(Q);
		DebugMatrix(W);
		// 寻找是否有j使得pj<=0,pj+qj<=0,wj<=0同时满足 
		for(j=0;j<6;j++)
		{
			if(P.GetAt(0,j)<=0 && P.GetAt(0,j)+Q.GetAt(0,j)<=0 && W.GetAt(0,j)<=0)
				return false;
		}
		// 不等式计算

		for(i=0;i<9;i++)
		{
			for(j=0;j<9;j++)
			{	
				if(i==j)
					continue;
				if(j<6&&i<6&&j>i)
				{
					a11=Q.GetAt(0,i);		a12=W.GetAt(0,i);
					a21=Q.GetAt(0,j);		a22=W.GetAt(0,j);
					c1=P.GetAt(0,i);		c2=P.GetAt(0,j);
				}
				if(j>=6&&i<6)
				{
					a11=Q.GetAt(0,i);		a12=W.GetAt(0,i);
					c1=P.GetAt(0,i);
					if(j==6)
					{
						a21=0;		a22=1;
						c2=0;
					}
					if(j==7)
					{
						a21=1;		a22=0;
						c2=0;
					}
					if(j==8)
					{
						a21=1;		a22=0;
						c2=-1;
					}
				}
				if(j>=6&&i>=6&&j>i)
				{
					if(j==7)
						GetTRange(tmin,tmax,0,0,P,Q,W,S,d,bThrough);
					if(j==8&&i==6)
						GetTRange(tmin,tmax,0,1,P,Q,W,S,d,bThrough);
					continue;
				}
				if(a11<0.0001&&a11>-0.0001 && a21<0.0001&&a21>-0.0001 || a12<0.0001&&a12>-0.0001 && a22<0.0001&&a22>-0.0001)
				{// 方程无解
					continue;
				}
				else
				{
					t=-(c1*a22-c2*a12)/(a11*a22-a21*a12);
					a=(c1*a21-c2*a11)/(a11*a22-a21*a12);
					if(a<0 || t<0 || t>1)		// 不合理的解
						continue;
					GetTRange(tmin,tmax,a,t,P,Q,W,S,d,bThrough);
				}		
			}
		}
	if(tmin<tmax)
	{
		temp=S+d*tmin;
		temp2=S+d*tmax;
		InsertSTable(p1,p2,temp,temp2);
		return true;
	}
	return false;
}

void GetTRange(double& tmin,double& tmax,double a,double t,CMatrix& p,CMatrix& q,CMatrix& w,CMatrix& s,CMatrix& d,bool bThrough)
{
	CMatrix temp(1,4);
	int j;
	for(j=0;j<6;j++)
	{
		if(p.GetAt(0,j) + q.GetAt(0,j)*t + w.GetAt(0,j)*a<-0.0001)
			return;
	}
	if(t<tmin)
		tmin=t;
	if(t>tmax)
		tmax=t;
	if(bThrough)
	{
		if(a<0.000001&&a>-0.000001)
		{
			temp=s+d*t;
			InsertCTPTable(temp.GetAt(0,0),temp.GetAt(0,1),temp.GetAt(0,2));
		}
	}
}

void InsertCTPTable(double x,double y,double z)
{
	CTPTable[cti].x=x+0.1;
	CTPTable[cti].y=y+0.1;
	CTPTable[cti].z=z+0.1;
	cti++;
}

void InsertSTable(int p1,int p2,CMatrix& MP1,CMatrix& MP2)
{
	STable[sti].x1=PTable[p1].x;
	STable[sti].x2=MP1.GetAt(0,0);
	STable[sti].y1=PTable[p1].y;
	STable[sti].y2=MP1.GetAt(0,1);
	STable[sti].z1=PTable[p1].z;
	STable[sti].z2=MP1.GetAt(0,2);
	sti++;
	STable[sti].x2=PTable[p2].x;
	STable[sti].x1=MP2.GetAt(0,0);
	STable[sti].y2=PTable[p2].y;
	STable[sti].y1=MP2.GetAt(0,1);
	STable[sti].z2=PTable[p2].z;
	STable[sti].z1=MP2.GetAt(0,2);
	sti++;
}

void AreaPoint(AreaNode& an)
{
	int size=an.ll.size();
	int i;
	int flag[30];
	for(i=0;i<30;i++)
		flag[i]=0;

	for(i=0;i<size;i++)
	{
		flag[LTable[an.ll[i]].p1]++;		
		flag[LTable[an.ll[i]].p2]++;
	}
	for(i=0;i<30;i++)
		if(flag[i]>0)
			an.pl.push_back(i);
}

void Init()
{
	int i=1;
	sti=cti=0;
	PTable[1].x=0;
	PTable[1].y=0;
	PTable[1].z=1;
	PTable[2].x=2;
	PTable[2].y=0;
	PTable[2].z=1;
	PTable[3].x=2;
	PTable[3].y=0;
	PTable[3].z=3;
	PTable[4].x=0;
	PTable[4].y=0;
	PTable[4].z=3;
	PTable[5].x=0;
	PTable[5].y=6;
	PTable[5].z=1;
	PTable[6].x=2;
	PTable[6].y=6;
	PTable[6].z=1;
	PTable[7].x=2;
	PTable[7].y=6;
	PTable[7].z=3;
	PTable[8].x=0;
	PTable[8].y=6;
	PTable[8].z=3;
	PTable[9].x=1;
	PTable[9].y=2;
	PTable[9].z=0;
	PTable[10].x=3;
	PTable[10].y=2;
	PTable[10].z=0;
	PTable[11].x=3;
	PTable[11].y=2;
	PTable[11].z=4;
	PTable[12].x=1;
	PTable[12].y=2;
	PTable[12].z=4;
	PTable[13].x=1;
	PTable[13].y=4;
	PTable[13].z=0;
	PTable[14].x=3;
	PTable[14].y=4;
	PTable[14].z=0;
	PTable[15].x=3;
	PTable[15].y=4;
	PTable[15].z=4;
	PTable[16].x=1;
	PTable[16].y=4;
	PTable[16].z=4;					

	for(;i<25;i++)
		LTable[i].bHide=false;
	g_PolyList.clear();
	PolyNode pn;
	AreaNode an;
	an.ll.push_back(2);		an.ll.push_back(11);		an.ll.push_back(6);		an.ll.push_back(10);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(4);		an.ll.push_back(12);		an.ll.push_back(8);		an.ll.push_back(9);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(5);		an.ll.push_back(6);		an.ll.push_back(7);		an.ll.push_back(8);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(1);		an.ll.push_back(2);		an.ll.push_back(3);		an.ll.push_back(4);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(3);		an.ll.push_back(12);		an.ll.push_back(7);		an.ll.push_back(11);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(1);		an.ll.push_back(10);		an.ll.push_back(5);		an.ll.push_back(9);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	pn.index=1;
	g_PolyList.push_back(pn);
	pn.al.clear();
	an.ll.push_back(14);		an.ll.push_back(23);		an.ll.push_back(18);		an.ll.push_back(22);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(21);		an.ll.push_back(20);		an.ll.push_back(24);		an.ll.push_back(16);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(17);		an.ll.push_back(18);		an.ll.push_back(19);		an.ll.push_back(20);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(13);		an.ll.push_back(14);		an.ll.push_back(15);		an.ll.push_back(16);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(15);		an.ll.push_back(24);		an.ll.push_back(19);		an.ll.push_back(23);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	an.ll.push_back(13);		an.ll.push_back(22);		an.ll.push_back(17);		an.ll.push_back(21);
	AreaPoint(an);
	pn.al.push_back(an);
	an.ll.clear();
	an.pl.clear();
	pn.index=2;
	g_PolyList.push_back(pn);
}

double Min(double x,double y)
{
	if(x>y)
		return y;
	else
		return x;
}

double Max(double x,double y)
{
	if(x>y)
		return x;
	else 
		return y;
}

// 设置边表和体表的Min和Max值
void SetMinMax()
{
	int i;
	for(i=1;i<25;i++)
	{
		LTable[i].XMax=Max(PTable[LTable[i].p1].x,PTable[LTable[i].p2].x);
		LTable[i].YMax=Max(PTable[LTable[i].p1].y,PTable[LTable[i].p2].y);
		LTable[i].ZMax=Max(PTable[LTable[i].p1].z,PTable[LTable[i].p2].z);

		LTable[i].XMin=Min(PTable[LTable[i].p1].x,PTable[LTable[i].p2].x);
		LTable[i].YMin=Min(PTable[LTable[i].p1].y,PTable[LTable[i].p2].y);
		LTable[i].ZMin=Min(PTable[LTable[i].p1].z,PTable[LTable[i].p2].z);
	}

	double xmin=1000,xmax=-1000,ymin=1000,ymax=-1000,zmin=1000,zmax=-1000;
	PolyList::iterator pli=g_PolyList.begin();
	AreaList::iterator ali;
	int size;
	int j;
	for(;pli!=g_PolyList.end();pli++)
	{
		xmin=1000;xmax=-1000;ymin=1000;ymax=-1000;zmin=1000;zmax=-1000;
		ali=pli->al.begin();
		for(;ali!=pli->al.end();ali++)
		{
				size=ali->pl.size();
				for(j=0;j<size;j++)
				{
					if(PTable[ali->pl[j]].x<xmin)
						xmin=PTable[ali->pl[j]].x;
					if(PTable[ali->pl[j]].x>xmax)
						xmax=PTable[ali->pl[j]].x;
					if(PTable[ali->pl[j]].y<ymin)
						ymin=PTable[ali->pl[j]].y;
					if(PTable[ali->pl[j]].y>ymax)
						ymax=PTable[ali->pl[j]].y;
					if(PTable[ali->pl[j]].z<zmin)
						zmin=PTable[ali->pl[j]].z;
					if(PTable[ali->pl[j]].z>zmax)
						zmax=PTable[ali->pl[j]].z;
				}
		}
		pli->XMin=xmin;
		pli->XMax=xmax;
		pli->YMin=ymin;
		pli->YMax=ymax;
		pli->ZMin=zmin;
		pli->ZMax=zmax;
	}
}

void Transform(CMatrix& mx)
{
	CMatrix tran1(4,4),tran2(4,4);
	double t1=cos(yDelta*PI/180.0);
	double t2=sin(yDelta*PI/180.0);
	tran1.SetIdentity();
	tran1.SetAt(0,0,t1);
	tran1.SetAt(0,2,t2);
	tran1.SetAt(2,0,-t2);
	tran1.SetAt(2,2,t1);
	t1=cos(xDelta*PI/180.0);
	t2=sin(xDelta*PI/180.0);
	tran2.Clear();
	tran2.SetIdentity();
	tran2.SetAt(1,1,t1);
	tran2.SetAt(1,2,t2);
	tran2.SetAt(2,1,-t2);
	tran2.SetAt(2,2,t1);
	mx=mx*tran1*tran2;
}

void DrawRaw(CDC& dc,double x1,double y1,double x2,double y2)
{
	x1=400+x1*100;
	y1=600-y1*100;
	x2=400+x2*100;
	y2=600-y2*100;
	dc.MoveTo((int)x1,(int)y1);
	dc.LineTo((int)x2,(int)y2);
	
}

void DrawRaw2(CDC& dc,double x1,double y1,double x2,double y2)
{
	x1=400+(x1-0.1)*100;
	y1=600-(y1-0.1)*100;
	x2=400+(x2-0.1)*100;
	y2=600-(y2-0.1)*100;
	dc.MoveTo((int)x1,(int)y1);
	dc.LineTo((int)x2,(int)y2);
	
}

void SetCTL()
{
	int d=0;
	int i,j;
	for(i=0;CTPTable[i].x!=0;i++)
	{
		for(j=0;j<i;j++)
		{
			CTLTable[d].bHide=false;
			CTLTable[d].p1=i;
			CTLTable[d].p2=j;
			d++;
		}
	}
}

void GetTRange2(double& tmin,double& tmax,double a,double t,CMatrix& p,CMatrix& q,CMatrix& w,CMatrix& s,CMatrix& d)
{
	int j;
	if(a<0.0001&&a>-0.0001)
		return;
	for(j=0;j<6;j++)
	{
		if( p.GetAt(0,j) +q.GetAt(0,j)*t+w.GetAt(0,j)*a <0.0)
			return;
	}
	if(t<tmin)
		tmin=t;
	if(t>tmax)
		tmax=t;

}
bool CompleteTest2(int LineNo,int p1,int p2,int index)
{
	PolyList::iterator pli=g_PolyList.begin();
	double tmin=1000,tmax=-1000;
	int i,j;
	CMatrix S(1,4),d(1,4),P(1,6),Q(1,6),W(1,6),g(1,4);
	CMatrix temp(1,4),temp2(1,4);
	double a11,a12,a21,a22,c1,c2;
	double a,t;
		S.SetAt(0,0,CTPTable[p1].x);
		S.SetAt(0,1,CTPTable[p1].y);
		S.SetAt(0,2,CTPTable[p1].z);
		S.SetAt(0,3,1);
		d.SetAt(0,0,CTPTable[p2].x-CTPTable[p1].x);
		d.SetAt(0,1,CTPTable[p2].y-CTPTable[p1].y);
		d.SetAt(0,2,CTPTable[p2].z-CTPTable[p1].z);
		d.SetAt(0,3,0);
		g.SetAt(0,0,0);		g.SetAt(0,1,0);		g.SetAt(0,2,1);		g.SetAt(0,3,0);
	if(index == 1)
	{
/*				if(CTLTable[LineNo].XMax < pli->XMin 
					|| CTLTable[LineNo].XMin > pli->XMax
					|| CTLTable[LineNo].YMax < pli->YMin
					|| CTLTable[LineNo].YMin > pli->YMax)
				{
					return false;
				}
				else if(CTLTable[LineNo].ZMin > pli->ZMax ||
					    CTLTable[LineNo].ZMax < pli->ZMin)
				{
					return false;
				}

*/
		P=S*area1;
		Q=d*area1;
		W=g*area1;
	}
	else
	{
/*		pli++;
				if(CTLTable[LineNo].XMax < pli->XMin 
					|| CTLTable[LineNo].XMin > pli->XMax
					|| CTLTable[LineNo].YMax < pli->YMin
					|| CTLTable[LineNo].YMin > pli->YMax)
				{
					return false;
				}
				else if(CTLTable[LineNo].ZMin > pli->ZMax ||
					    CTLTable[LineNo].ZMax < pli->ZMin)
				{
					return false;
				}
*/
		P=S*area2;
		Q=d*area2;
		W=g*area2;
	}
		TRACE("\n");
		DebugMatrix(area2);
		DebugMatrix(area1);
		DebugMatrix(S);	
		DebugMatrix(d);
		DebugMatrix(P);
		DebugMatrix(Q);
		DebugMatrix(W);
		// 寻找是否有j使得pj<=0,pj+qj<=0,wj<=0同时满足 
		for(j=0;j<6;j++)
		{
			if(P.GetAt(0,j)<=0.0 && P.GetAt(0,j)+Q.GetAt(0,j)<=0.0 && W.GetAt(0,j)<=0.0)
				return false;
		}
		// 不等式计算

		for(i=0;i<9;i++)
		{
			for(j=0;j<9;j++)
			{	
				if(i==j)
					continue;
				if(j<6&&i<6&&j>i)
				{
					a11=Q.GetAt(0,i);		a12=W.GetAt(0,i);
					a21=Q.GetAt(0,j);		a22=W.GetAt(0,j);
					c1=P.GetAt(0,i);		c2=P.GetAt(0,j);
				}
				if(j>=6&&i<6)
				{
					a11=Q.GetAt(0,i);		a12=W.GetAt(0,i);
					c1=P.GetAt(0,i);
					if(j==6)
					{
						a21=0;		a22=1;
						c2=0;
					}
					if(j==7)
					{
						a21=1;		a22=0;
						c2=0;
					}
					if(j==8)
					{
						a21=1;		a22=0;
						c2=-1;
					}
				}
				if(j>=6&&i>=6&&j>i)
				{
					if(j==7)
						GetTRange2(tmin,tmax,0,0,P,Q,W,S,d);
					if(j==8&&i==6)
						GetTRange2(tmin,tmax,0,1,P,Q,W,S,d);
					continue;
				}
				if(a11<0.0001&&a11>-0.0001 && a21<0.0001&&a21>-0.0001 || a12<0.0001&&a12>-0.0001 && a22<0.0001&&a22>-0.0001)
				{// 方程无解
					continue;
				}
				else
				{
					t=-(c1*a22-c2*a12)/(a11*a22-a21*a12);
					a=(c1*a21-c2*a11)/(a11*a22-a21*a12);
					if(a<0.0 || t<0.0 || t>1.0)		// 不合理的解
						continue;
					GetTRange2(tmin,tmax,a,t,P,Q,W,S,d);
				}		
			}
		}
//	if(tmin<0.00001 && tmin>-0.00001 && tmax <1.00001 &&tmax > 0.999999)
	if(tmin<=tmax)
		return true;
	else
		return false;
}



void Draw(CDC& dc)
{
	int i=1;
	int j;
	for(;i<25;i++)
	{
		if(LTable[i].bHide==false)
		{
			DrawLine(dc,LTable[i].p1,LTable[i].p2);
		}
	}
	for(i=0;STable[i].x1!=0;i++)
	{
		DrawRaw(dc,STable[i].x1,STable[i].y1,STable[i].x2,STable[i].y2);
	}
	SetCTL();
	for(i=0;CTPTable[i].x!=0;i++)
		TRACE("%f,%f,%f\n",CTPTable[i].x,CTPTable[i].y,CTPTable[i].z);
	for(i=1;CTLTable[i].p1!=0;i++)
	{
		CTLTable[i].XMax=Max(CTPTable[CTLTable[i].p1].x,CTPTable[CTLTable[i].p2].x);
		CTLTable[i].YMax=Max(CTPTable[CTLTable[i].p1].y,CTPTable[CTLTable[i].p2].y);
		CTLTable[i].ZMax=Max(CTPTable[CTLTable[i].p1].z,CTPTable[CTLTable[i].p2].z);

		CTLTable[i].XMin=Min(CTPTable[CTLTable[i].p1].x,CTPTable[CTLTable[i].p2].x);
		CTLTable[i].YMin=Min(CTPTable[CTLTable[i].p1].y,CTPTable[CTLTable[i].p2].y);
		CTLTable[i].ZMin=Min(CTPTable[CTLTable[i].p1].z,CTPTable[CTLTable[i].p2].z);
	}
	
	for(i=0;CTLTable[i].p1!=0;i++)
	{
//		if(CTLTable[i].p1==4&&CTLTable[i].p2==0)
		if(CompleteTest2(i,CTLTable[i].p1,CTLTable[i].p2,1) || CompleteTest2(i,CTLTable[i].p1,CTLTable[i].p2,2))
//			if(CompleteTest2(i,CTLTable[i].p1,CTLTable[i].p2,1))
				CTLTable[i].bHide=true;
	}
	for(i=0;CTLTable[i].p1!=0;i++)
	{
//				if(CTLTable[i].p1==4&&CTLTable[i].p2==0)
					if(CTLTable[i].bHide==false)
		{
			DrawRaw2(dc,CTPTable[CTLTable[i].p1].x,CTPTable[CTLTable[i].p1].y,CTPTable[CTLTable[i].p2].x,CTPTable[CTLTable[i].p2].y);
		}
	}

}

void DrawLine(CDC& dc,int p1,int p2)
{
	double x1=PTable[p1].x;
	double y1=PTable[p1].y;
	double x2=PTable[p2].x;
	double y2=PTable[p2].y;
	x1=400+x1*100;
	y1=600-y1*100;
	x2=400+x2*100;
	y2=600-y2*100;
	dc.MoveTo((int)x1,(int)y1);
	dc.LineTo((int)x2,(int)y2);
}

void DebugMatrix(CMatrix& temp)
{
	for(int i=0;i<temp.GetRowDim();i++){
		for(int j=0;j<temp.GetColDim();j++)
			TRACE("%f\t",temp.GetAt(i,j));
		TRACE("\n");
	}
	TRACE("\n");

}

