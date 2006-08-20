// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView
IMPLEMENT_DYNCREATE(CChildView,CHtmlView)
CChildView::CChildView()//:m_Dibfile(CSize(800,600),24)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CHtmlView )
	//{{AFX_MSG_MAP(CChildView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CChildView::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	LoadFromResource( IDR_WELCOME );
}

void CChildView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}
#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG};
/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers



