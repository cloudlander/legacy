// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Graph.h"
#include "ChildView.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool operator==(Line& l1,Line& l2){
    if(l1.x == l2.x && l1.dx == l2.dx && l1.dy == l2.dy)
         return true;
    else
         return false;
}


bool operator<(YLineList& y1,YLineList& y2){
    if(y1.y < y2.y )
        return true;
    else
        return false;
}



/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_bDrawing=false;
	m_bCapture=false;
	m_BackClr=RGB(0,0,0);
	m_FillClr=RGB(255,255,255);

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
    //{{AFX_MSG_MAP(CChildView)
    ON_WM_PAINT()
	ON_COMMAND(ID_START, OnStart)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_START, OnUpdateStart)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_WM_CREATE()
	ON_COMMAND(ID_BACKCLR, OnBackclr)
	ON_COMMAND(ID_FRONTCLR, OnFrontclr)
	ON_UPDATE_COMMAND_UI(ID_BACKCLR, OnUpdateBackclr)
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
	dc.BitBlt(0,0,m_Cx,m_Cy,&m_MemDC,0,0,SRCCOPY);
    
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages
}

void SortToYBucket(YBucket& ybkt,YLineList& yl)
{
    if(0 == ybkt.size()){
        ybkt.push_front(yl);
		return;
	}
    YBucket::iterator N = ybkt.begin();
    while(N!=ybkt.end()){
        if( yl < (*N))
            N++;
        else if((*N) < yl){
            ybkt.insert(N,yl);
            return;
        }else
			return;
    }
	ybkt.push_back(yl);
}

LineList* GetYLineList(YBucket& ybt,int y){
	YBucket::iterator N=ybt.begin();
	while(N!=ybt.end()){
		if(y == (*N).y)
			return &((*N).yylist);
		else
			N++;
	}
	return NULL;
}

void CChildView::FillDraw(CPoint pArray[],int num,CDC* pdc,COLORREF clr)
{
    // Y桶数据准备
    LineList*            pllist;
    YBucket           lbukt;
    Line*         pll;
    YLineList*        pylist;
    // 顺序扫描pArray,约定顶点按反时针
    int i;
    int ibef;   // 顶点之前
    int ifol;   // 顶点之后
    int ytemp;
    for( i=0;i<num;i++)
    {
		bool bhas=true;
        ibef = (i+1)%num;
        ifol = (i-1+num)%num;
        ytemp = (int)pArray[i].y;
        if( pArray[i].y > pArray[ibef].y && pArray[i].y > pArray[ifol].y ){

            pllist=GetYLineList(lbukt,ytemp);
            if(pllist == NULL){
				bhas=false;
				pllist = new LineList;
			}
          pll = new Line;
            pll->x=pArray[i].x;
            pll->dy=(int)(pArray[i].y-pArray[ifol].y);
            pll->dx=((float)(pArray[i].x-pArray[ifol].x)) / ((float) (pArray[ifol].y-pArray[i].y));
			pllist->push_back(*pll);
			delete pll;
            pll = new Line;
			pll->x=pArray[i].x;
            pll->dy=(int)(pArray[i].y-pArray[ibef].y);
            pll->dx=((float)(pArray[ibef].x-pArray[i].x)) / ((float)(pArray[i].y-pArray[ibef].y));
            pllist->push_back(*pll);
			delete pll;


            
        }
        else if( pArray[i].y <= pArray[ifol].y && pArray[i].y > pArray[ibef].y ){

            pllist=GetYLineList(lbukt,ytemp);
            if(pllist == NULL){
				bhas=false;
				pllist = new LineList;
			}

            pll = new Line;
            pll->x = pArray[i].x;
            pll->dy = (int)(pArray[i].y-pArray[ibef].y);
            pll->dx=((float)(pArray[i].x-pArray[ibef].x)) / ((float)(pArray[ibef].y-pArray[i].y));
            pllist->push_back(*pll);
			delete pll;
            
            
        }
        else if( pArray[i].y > pArray[ifol].y && pArray[i].y <= pArray[ibef].y ){

            pllist=GetYLineList(lbukt,ytemp);
            if(pllist == NULL){
				bhas=false;
				pllist =  new LineList;
			}
            pll = new Line;
            pll->x = pArray[i].x;
            pll->dy = (int)(pArray[i].y-pArray[ifol].y);
            pll->dx=((float)(pArray[i].x-pArray[ifol].x)) / ((float)(pArray[ifol].y-pArray[i].y));
            
            pllist->push_back(*pll);
           delete pll;
            
        }
        else
            continue;
		if(!bhas){
			pylist=new YLineList(*pllist,ytemp);
			delete pllist;
			SortToYBucket(lbukt,*pylist);   
			delete pylist;
		}
		
    }
/*
    YBucket::const_iterator ybb = lbukt.begin();
    YList::const_iterator   yl;
    while(ybb!=lbukt.end()){
        yl=(*ybb).yylist.begin();
        while(yl!=(*ybb).yylist.end()){
//            TRACE("Point: %d,%f,%d\n",(*yl).x,(*yl).dx,(*yl).dy);
			yl++;
		}
		ybb++;
    }
/*/  

	int y,ymin,ymax;
	LineList ActiveLine;
	bool  bActive;
	int x1,x2;
	YBucket::iterator ybi = lbukt.begin();
	LineList::iterator   ylt ;

	CPen* pTempPen;
	CPen pen(PS_SOLID,0,clr);
	pTempPen=pdc->SelectObject(&pen);

	while(ybi!=lbukt.end()){
		bActive=true;
		ymax=(*ybi).y;
		ylt=(*ybi).yylist.begin();
		while(ylt!=(*ybi).yylist.end()){
			ActiveLine.push_back(*ylt);
			ylt++;
		}
		ybi++;
		if(ybi==lbukt.end())
			ymin=0;
		else
			ymin=(*ybi).y;

		for(y=ymax;y>ymin  ;y--){
			ActiveLine.sort();
			ylt=ActiveLine.begin();
			while(ylt!=ActiveLine.end()){
					x1=(*ylt).x;
					(*ylt).x=(float)(*ylt).x+(*ylt).dx;
					(*ylt).dy-=1;

					if((*ylt).dy==0){
					bActive=false;
					}


					ylt++;
					x2=(*ylt).x;
					(*ylt).x=(float)(*ylt).x+(*ylt).dx;
					(*ylt).dy-=1;

					if((*ylt).dy==0){
					bActive=false;
					}
					ylt++;
					pdc->MoveTo(x1,y);
					pdc->LineTo(x2,y);
/*						
					for(i=x1;i<=x2;i++){
//						TRACE("Point: %d,%d\n",i,y);
						pdc->SetPixel(i,y,clr);
					}
*/
			 }
			
				if(!bActive){
					ylt=ActiveLine.begin();
					while(ylt!=ActiveLine.end()){
						if((*ylt).dy==0){
							ActiveLine.erase(ylt);
							ylt=ActiveLine.begin();
							
						}
						else
							ylt++;
					}
				}

			
			
		}

		
	}
	pdc->SelectObject(pTempPen);
/*
	//clearing:
	ybi=lbukt.begin();
	while(ybi!=lbukt.end()){
//		delete &((*ybi).yylist);
		delete &(*ybi);
//		lbukt.erase(ybi);
//		ybi=lbukt.begin();
		ybi++;
	}
*/	

}

void CChildView::OnStart() 
{
	// TODO: Add your command handler code here
	m_bDrawing=true;	
	m_Count=0;
	m_bCanClose=false;
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDC* pdc=GetDC();
//	CClientDC cldc(this);
	
	int i,j;
	CRect rect;
	if(m_bDrawing)
	{
		
		pdc->DPtoLP(&point,1);
		if(!m_bCanClose)
		{
			if(m_Count>1) m_bCanClose=true;
		}
		else{
			for(i=0;i<m_Count-1;i++){
				rect=CRect(m_Poly[i].x-3,m_Poly[i].y-3,m_Poly[i].x+3,m_Poly[i].y+3);
				if(rect.PtInRect(point)){
					CPen* pTempPen;
					m_Poly[m_Count]=point;
//					TRACE("START Fill Draw\n");		
					m_MemDC.SetROP2(R2_XORPEN);
					CPen pen(PS_SOLID,0,m_FillClr);
					pTempPen=m_MemDC.SelectObject(&pen);
					m_MemDC.MoveTo(m_Poly[0]);
					for(j=1;j<=i;j++){
						m_MemDC.LineTo(m_Poly[j]);
					}
					m_MemDC.SetROP2(R2_COPYPEN);
					FillDraw(m_Poly+i,m_Count+1-i,&m_MemDC,m_FillClr);
					Invalidate(FALSE);
					m_Count=0;
					m_bCapture=false;
					return;
				}
			}
		}
		m_Poly[m_Count++]=point;
		m_bCapture=true;
		if(m_Count==1)  m_folPoint=point;
		m_MemDC.SetPixel(point,m_FillClr);
		Invalidate(FALSE);
//		TRACE("Point of %d:(%d,%d)",m_Count,point.x,point.y);
		
	}
		
//	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CClientDC dc(this);
	CPen* pTempPen;
	if(m_bDrawing && m_bCapture){
		m_MemDC.DPtoLP(&point,1);
		m_MemDC.SetROP2(R2_XORPEN);
		CPen pen(PS_SOLID,0,m_FillClr);
		pTempPen=m_MemDC.SelectObject(&pen);
		m_MemDC.MoveTo(m_Poly[m_Count-1]);
		m_MemDC.LineTo(m_folPoint);
		Invalidate(FALSE);
		m_MemDC.MoveTo(m_Poly[m_Count-1]);
		m_MemDC.LineTo(point);
		Invalidate(FALSE);
		m_folPoint=point;
		m_MemDC.SelectObject(pTempPen);
	}
//	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnStop() 
{
	// TODO: Add your command handler code here
	m_bDrawing=false;
	m_bCapture=false;
	CBrush brush(m_BackClr);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,m_Cx,m_Cy,PATCOPY);
	Invalidate(FALSE);
}



void CChildView::OnUpdateStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bDrawing);
}

void CChildView::OnUpdateStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bDrawing);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_Cx=::GetSystemMetrics(SM_CXSCREEN);
	m_Cy=::GetSystemMetrics(SM_CYSCREEN);
	CPaintDC dc(this);
	HBRUSH brush=(HBRUSH)::GetStockObject(BLACK_BRUSH);
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,m_Cx,m_Cy);
	m_MemDC.SelectObject(&m_Bitmap);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,m_Cx,m_Cy,PATCOPY);
	
	return 0;
}

void CChildView::OnBackclr() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd;
	if(IDOK == ccd.DoModal()){
		m_BackClr=ccd.GetColor();
		CBrush brush(m_BackClr);
		m_MemDC.SelectObject(brush);
		m_MemDC.PatBlt(0,0,m_Cx,m_Cy,PATCOPY);
		Invalidate(FALSE);
	}


}

void CChildView::OnFrontclr() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd;
	if(IDOK == ccd.DoModal())
		m_FillClr=ccd.GetColor();
	
}

void CChildView::OnUpdateBackclr(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bDrawing);
}
