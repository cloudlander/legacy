// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "MainFrm.h"
#include "ScanLineView.h"
#include "3DView.h"
#include "FloatView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
static CRuntimeClass* PrjList [] = {
	RUNTIME_CLASS(CTeapotView),
	RUNTIME_CLASS(CVasView),
	RUNTIME_CLASS(CScanLineView),
	RUNTIME_CLASS(C3DView),
	RUNTIME_CLASS(CFloatView)

};
static Bar	BarList [] = {
	{ IDR_TEAPOT_TOOLBAR, FALSE },
	{ IDR_VAS_TOOLBAR,	FALSE },
	{ IDR_SCAN_TOOLBAR, FALSE },
	{ IDR_3D_TOOLBAR,	FALSE },
	{ IDR_FLOAT_TOOLBAR,FALSE }
};

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_PRJ, OnViewPrj)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRJ, OnUpdateViewPrj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bShowList = TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CCJFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
/*	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
*/
 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
 
	if (!m_TeapotBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_TEAPOT_TOOLBAR) ||
		!m_TeapotBar.LoadToolBar(IDR_TEAPOT_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_VasBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_VAS_TOOLBAR) ||
		!m_VasBar.LoadToolBar(IDR_VAS_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_ScanBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_SCAN_TOOLBAR) ||
		!m_ScanBar.LoadToolBar(IDR_SCAN_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_3DBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_3D_TOOLBAR) ||
		!m_3DBar.LoadToolBar(IDR_3D_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_FloatBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_FLOAT_TOOLBAR) ||
		!m_FloatBar.LoadToolBar(IDR_FLOAT_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndMenuBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndMenuBar.LoadMenu(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }
	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_TeapotBar.EnableDocking(CBRS_ALIGN_LEFT);
	m_VasBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ScanBar.EnableDocking(CBRS_ALIGN_ANY);
	m_3DBar.EnableDocking(CBRS_ALIGN_ANY);
	m_FloatBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	
	DockControlBarLeftOf((CToolBar*)&m_VasBar,(CToolBar*)&m_wndToolBar);
	DockControlBarLeftOf((CToolBar*)&m_ScanBar,(CToolBar*)&m_wndToolBar);
	DockControlBarLeftOf((CToolBar*)&m_TeapotBar,(CToolBar*)&m_wndToolBar);
	DockControlBarLeftOf((CToolBar*)&m_3DBar,(CToolBar*)&m_wndToolBar);
	DockControlBarLeftOf((CToolBar*)&m_FloatBar,(CToolBar*)&m_wndToolBar);
//	FloatControlBar(&m_TeapotBar,CPoint(500,200));
//	FloatControlBar(&m_ScanBar,CPoint(800,300));
	OnBarCheck(IDR_TEAPOT_TOOLBAR);
	OnBarCheck(IDR_VAS_TOOLBAR);
	OnBarCheck(IDR_SCAN_TOOLBAR);
	OnBarCheck(IDR_3D_TOOLBAR);
	OnBarCheck(IDR_FLOAT_TOOLBAR);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU|
		WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	cs.lpszClass = AfxRegisterWndClass(0,0,0,AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
//	m_wndView.SetFocus();
	m_wndSplitter.GetPane(0,1)->SetFocus();
}
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndSplitter.GetPane(0,1)->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CFrameWnd::OnCreateClient(lpcs, pContext);

    if (!m_wndSplitter.CreateStatic(this, 1, 2))
    {
        TRACE0("Failed to create splitter window\n");
        return FALSE;
    }
    
    if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CChildView),
        CSize(0, 0), pContext))
    {
        TRACE0("Failed to create CGraphStudioView\n"); 
        return FALSE;
    }
    if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CPrjListView),
        CSize(0, 0), pContext))
    {
        TRACE0("Failed to create CPrjListView\n"); 
        return FALSE;
    }
    // Standard sizing for splitter
    CRect r;
    GetClientRect(&r);
    int w1 = r.Width()/6;
    int w2 = r.Width()/5;
    m_wndSplitter.SetColumnInfo( 0, w1, 0 );
    m_wndSplitter.SetColumnInfo( 1, w2, 0 );
    m_wndSplitter.RecalcLayout();
    return true;
}



void CMainFrame::ChangeView(int i)
{
	SetActiveView(NULL,FALSE);
	static int temp = -1;
	m_wndSplitter.GetPane(0,1)->SendMessage(WM_CLOSE);
	CControlBar* pcb;
	if (-1 == i)
	{
	    if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CChildView),
		    CSize(0, 0), NULL))
		{
			TRACE0("Failed to create CGraphStudioView\n"); 
			ASSERT(FALSE);
		}
		if(temp != -1)
		{
			pcb = GetControlBar(BarList[temp].id);
			ShowControlBar(pcb,FALSE,FALSE);
			BarList[temp].visible = FALSE;
		}
	}
	else
	{
		if (!m_wndSplitter.CreateView(0, 1, PrjList[i],
			CSize(0, 0), NULL))
		{
			TRACE0("Failed to create CGraphStudioView\n"); 
			ASSERT(FALSE);
		}
		
		if( temp == -1 )
		{
			pcb = GetControlBar(BarList[i].id);
			ShowControlBar(pcb,TRUE,FALSE);
			BarList[i].visible = TRUE;
		}
		else
		{
			pcb = GetControlBar(BarList[temp].id);
			ShowControlBar(pcb,FALSE,FALSE);
			BarList[temp].visible = FALSE;
			pcb = GetControlBar(BarList[i].id);
			ShowControlBar(pcb,TRUE,FALSE);
			BarList[i].visible = TRUE;
		}
		temp = i;
	}


    m_wndSplitter.RecalcLayout();
	OnSetFocus(NULL);
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
 // 设置工具条并列停靠在同一条边上
 CRect rect; //矩形区域定义
 DWORD dw;
 UINT n=0;
 RecalcLayout(); //重新显示
 LeftOf->GetWindowRect(&rect);
 rect.OffsetRect(30,0); //设置偏移植以停靠在同一边上
 dw=LeftOf->GetBarStyle();
 n=(dw&CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
 n=(dw&CBRS_ALIGN_BOTTOM&&n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
 n=(dw&CBRS_ALIGN_LEFT&&n==0)?AFX_IDW_DOCKBAR_LEFT:n;
 n=(dw&CBRS_ALIGN_RIGHT&&n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
 DockControlBar(Bar,n,&rect);
}


void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_MAXIMIZE;
	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnViewPrj() 
{
	// TODO: Add your command handler code here
    CRect r;
    GetClientRect(&r);
    int w1 ;
    int w2 ;
	if(m_bShowList)
	{
		w1 = r.Width()/1000;
		m_bShowList = FALSE;
	}
	else
	{
		w1 = r.Width()/6;
		m_bShowList = TRUE;
	}
		
	w2 = r.Width()/5;
    m_wndSplitter.SetColumnInfo( 0, w1, 0 );
    m_wndSplitter.SetColumnInfo( 1, w2, 0 );
    m_wndSplitter.RecalcLayout();
}

void CMainFrame::OnUpdateViewPrj(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowList);
}
