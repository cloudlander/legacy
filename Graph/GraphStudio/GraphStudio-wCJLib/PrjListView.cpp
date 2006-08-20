// PrjListView.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "PrjListView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define	IDC_TREE_VIEW	200
/////////////////////////////////////////////////////////////////////////////
// CPrjListView
IMPLEMENT_DYNCREATE(CPrjListView,CWnd)
CPrjListView::CPrjListView()
{
}

CPrjListView::~CPrjListView()
{
}


BEGIN_MESSAGE_MAP(CPrjListView, CWnd)
	//{{AFX_MSG_MAP(CPrjListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VIEW, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrjListView message handlers

int CPrjListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// add items to the tree control.
	// create the tree control.
	if (!m_TreeCtrl.Create(WS_VISIBLE, CRect(0,0,0,0), this, IDC_TREE_VIEW))
	{
		TRACE0(_T("Unable to create tree control.\n"));
		return -1;
	}

	HTREEITEM hti = m_TreeCtrl.InsertItem("Graphic Scene", 0, 0);
	m_TreeCtrl.SetItemData(hti,DWORD(-1));
	HTREEITEM hts;
	hts = m_TreeCtrl.InsertItem( "茶壶",		 2,  2, hti);
	m_TreeCtrl.SetItemData(hts,DWORD(0));
	hts = m_TreeCtrl.InsertItem( "位图演示",		 3,  3, hti);
	m_TreeCtrl.SetItemData(hts,DWORD(1));
	hts = m_TreeCtrl.InsertItem( "扫描线算法",  7,  7, hti);
	m_TreeCtrl.SetItemData(hts,DWORD(2));
	hts = m_TreeCtrl.InsertItem( "3D场景模拟",		 8,  8, hti);
	m_TreeCtrl.SetItemData(hts,DWORD(3));
	hts = m_TreeCtrl.InsertItem( "浮动水平线算法",			 1,  1, hti);
	m_TreeCtrl.SetItemData(hts,DWORD(4));

	m_TreeCtrl.Expand( hti, TVE_EXPAND );
//	CMainFrame* pFrame = reinterpret_cast<CMainFrame* >(AfxGetMainWnd());
//	m_TreeCtrl.SetParent(pFrame);	
	return 0;	
}

void CPrjListView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_TreeCtrl.GetSafeHwnd()) {
		m_TreeCtrl.MoveWindow( 0, 1, cx, cy-1 );
	}
	
}

void CPrjListView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
 
 	HTREEITEM htItem = m_TreeCtrl.GetSelectedItem();
 	CMainFrame* pFrame = reinterpret_cast<CMainFrame* >(AfxGetMainWnd());
 	pFrame->ChangeView((int)m_TreeCtrl.GetItemData(htItem));
 	*pResult = 0;

}
