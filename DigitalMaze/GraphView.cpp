// GraphView.cpp : implementation of the CGraphView class
//

#include "stdafx.h"
#include "Graph.h"
#include <stdlib.h>
#include "GraphDoc.h"
#include "GraphView.h"
#include "Resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CView)

BEGIN_MESSAGE_MAP(CGraphView, CView)
	//{{AFX_MSG_MAP(CGraphView)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BUTTON,CGraphView::OnButton)
	// only placed this user defined Macro out of Afx_msg_map 
	// you can correctly open the Class Wizard dialog. hehe
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphView construction/destruction

CGraphView::CGraphView(): bAgain(false),nSecond(0),bStop(false)
{
	
	InitPlaceMatric();
	if(false==InitGraph())  AfxMessageBox("Error Initialize Graph");
	if(false==InitMemDC())
		AfxMessageBox("Error Initialize Memory");
}

CGraphView::~CGraphView()
{
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView drawing

void CGraphView::OnDraw(CDC* pDC)
{
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	//for(int j,int i=0;i<3;i++)
	//	for(j=0;j<3;j++){
	//		pDC->BitBlt(40+j*54,40+i*54,50,50,&memdc[i][j],0,0,SRCCOPY);
	//	}
	for(int i,j=0;j<3;j++){
	   for(i=j*3;i<3+j*3;i++)
		   pDC->BitBlt(40+(i%3)*54,40+j*54,50,50,&memdc[i],0,0,SRCCOPY);
	}
	if(bAgain) return;
    if(isSuccess())	
       PlayAgain();	
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView diagnostics

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphDoc* CGraphView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
	return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphView message handlers

bool CGraphView::InitGraph()
{
//	for(int i=0;i<9;i++)
//		IDB[i]=IDB_BITMAP1+i;
	int r;
	time_t t;
	srand((unsigned)time(&t));
	IDB[0]=rand()%9 + IDB_BITMAP1;
	for(int i=1;i<9;){
		r=rand()%9;
		for(int j=0;j<i&&r!=IDB[j]-IDB_BITMAP1;j++);
		if(j==i) {
			IDB[i]=r + IDB_BITMAP1;
			i++;
		}
	}
	for(i=0;i<9;i++)
		if(IDB_BITMAP9==IDB[i])
			CurrentPlace=i;
    return true;
}

void CGraphView::ChangeGraph()
{ 
    int i=-1;
	if(dir>=0)
	   i=place[CurrentPlace].dir[dir];
    int temp;
    if(i>=0){
        temp=IDB[i];
		IDB[i]=IDB[CurrentPlace];
		IDB[CurrentPlace]=temp;
		//bitmap.DeleteObject();
		bitmap.LoadBitmap(IDB[i]);
		memdc[i].SelectObject(&bitmap);
        bitmap.DeleteObject();
		bitmap.LoadBitmap(IDB[CurrentPlace]);
		memdc[CurrentPlace].SelectObject(&bitmap);	
		bitmap.DeleteObject();
		CurrentPlace=i;
		Invalidate(false);
		
	}
}

bool CGraphView::isSuccess()
{
	int nSuccess=0;
	for(int i=0;i<9;i++){
		if(i==IDB[i]-IDB_BITMAP1)
			nSuccess++;
	}
	if(nSuccess==9) return true;
	else  return false;
}

void CGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{		
	if(bAgain) bAgain=false;

	switch(nChar)	{
	case VK_UP:
		dir=2;
		break;
	case VK_DOWN:
		dir=0;
		break;
	case VK_LEFT:
		dir=1;
		break;
	case VK_RIGHT:
		dir=3;
		break;
	default:
		dir=-1;
	}
	ChangeGraph();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGraphView::InitPlaceMatric()
{
	place[0].dir[0]=-1;
	place[0].dir[1]=1;
	place[0].dir[2]=3;
	place[0].dir[3]=-1;
	place[1].dir[0]=-1;
	place[1].dir[1]=2;
	place[1].dir[2]=4;
	place[1].dir[3]=0;
	place[2].dir[0]=-1;
	place[2].dir[1]=-1;
	place[2].dir[2]=5;
	place[2].dir[3]=1;
	place[3].dir[0]=0;
	place[3].dir[1]=4;
	place[3].dir[2]=6;
	place[3].dir[3]=-1;
	place[4].dir[0]=1;
	place[4].dir[1]=5;
	place[4].dir[2]=7;
	place[4].dir[3]=3;
	place[5].dir[0]=2;
	place[5].dir[1]=-1;
	place[5].dir[2]=8;
	place[5].dir[3]=4;
	place[6].dir[0]=3;
	place[6].dir[1]=7;
	place[6].dir[2]=-1;
	place[6].dir[3]=-1;
	place[7].dir[0]=4;
	place[7].dir[1]=8;
	place[7].dir[2]=-1;
	place[7].dir[3]=6;
	place[8].dir[0]=5;
	place[8].dir[1]=-1;
	place[8].dir[2]=-1;
	place[8].dir[3]=7;
	
}

bool CGraphView::InitMemDC()
{
	CDC dc;
	for(int i=0;i<9;i++){
        memdc[i].CreateCompatibleDC(&dc);
		bitmap.LoadBitmap(IDB[i]);
		memdc[i].SelectObject(&bitmap);
		bitmap.DeleteObject();
	}
	return true;
}

void CGraphView::PlayAgain()
{			
	bAgain=true;
    CString str;
	str.Format("You Win!\nYou got it in %d seconds\nPlay Again?",nSecond);
//hehe, here is a strange thing I have encounted. if str is definded 
//as char[], than an Access Violation will occured.
//	sprintf(str,"You Win!\nYou got it in %d seconds\nPlay Again?",100);
//	if(true==isSuccess())
		if(IDYES==AfxMessageBox(str,MB_YESNO)){
			InitGraph();
			for(int i=0;i<9;i++){
				bitmap.LoadBitmap(IDB[i]);
				memdc[i].SelectObject(&bitmap);
				bitmap.DeleteObject();
			}
		nSecond=0;
		m_static.SetWindowText("Starting");
		CRect rect(0,0,200,200);
		InvalidateRect(rect,false);
		}
		
}



int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if(0==button.Create("重来",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(400,30,480,70),this,ID_BUTTON))
		AfxMessageBox("Error Initialize Button");
    m_static.Create("Starting",WS_CHILD|WS_VISIBLE,CRect(400,100,480,120),GetParent());
	
	SetTimer(1,1000,NULL);
	return 0;
}

void CGraphView::OnButton()
{
	bStop=true;
    AfxMessageBox("真的重来吗？\n");
    InitGraph();
	for(int i=0;i<9;i++){
		bitmap.LoadBitmap(IDB[i]);
		memdc[i].SelectObject(&bitmap);
		bitmap.DeleteObject();
	}
	nSecond=0;
	bStop=false;
	m_static.SetWindowText("Starting");
    CRect rect(0,0,200,200);
	InvalidateRect(rect,false);
}

void CGraphView::OnTimer(UINT nIDEvent) 
{
	char str1[10];
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox("Timer has been Installed");
	if(bStop)
		sprintf(str1,"Stopped");
	else
		sprintf(str1,"%d seconds",nSecond++);
	
	m_static.SetWindowText(str1);
	CView::OnTimer(nIDEvent);
}





void CGraphView::OnDestroy() 
{
	CView::OnDestroy();
	
	KillTimer(1);
	
}
