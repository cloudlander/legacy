// JobProgresser.cpp : implementation file
//

#include "stdafx.h"
#include "wmd5.h"
#include "JobProgresser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobProgresser

CJobProgresser::CJobProgresser()
{
}

CJobProgresser::~CJobProgresser()
{
}


BEGIN_MESSAGE_MAP(CJobProgresser, CProgressCtrl)
	//{{AFX_MSG_MAP(CJobProgresser)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobProgresser message handlers
/////////////////////////////////////////////////////////////////////////////
// CQueueProgresser

CQueueProgresser::CQueueProgresser()
{
}

CQueueProgresser::~CQueueProgresser()
{
}


BEGIN_MESSAGE_MAP(CQueueProgresser, CProgressCtrl)
	//{{AFX_MSG_MAP(CQueueProgresser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueueProgresser message handlers
/* user resizing is disable currently */
void CJobProgresser::OnSize(UINT nType, int cx, int cy) 
{
	/*
	CRect rc;
	GetParent()->GetClientRect(&rc);
	MoveWindow(10+rc.left,10+rc.top,rc.right-20,rc.bottom-30);
	this->SetRange(0,100);
	this->SetPos(78);
//	TRACE("%d,%d,%d,%d\n",rc.left,rc.top,rc.right,rc.bottom);
	CProgressCtrl::OnSize(nType, cx, cy);
	*/
	// TODO: Add your message handler code here
	
}

/* p <= 100 && p >= 0 */
void CJobProgresser::SetNewPos(int i,int p)
{
	CString text;
	text.Format("%d%%",p);
	m_pJobList->SetItemText(i,0,text);
}
