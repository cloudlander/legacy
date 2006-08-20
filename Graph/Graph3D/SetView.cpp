// SetView.cpp : implementation file
//

#include "stdafx.h"
#include "Graph3D.h"
#include "SetView.h"
#include "MainFrm.h"
#include "ChildView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetView
extern int Cx,Cy;

IMPLEMENT_DYNCREATE(CSetView, CFormView)

CSetView::CSetView()
	: CFormView(CSetView::IDD)
{
	//{{AFX_DATA_INIT(CSetView)
	//}}AFX_DATA_INIT
}

CSetView::~CSetView()
{
}

void CSetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetView)
	DDX_Control(pDX, IDC_SLIDER9, m_ctlVUPz);
	DDX_Control(pDX, IDC_SLIDER8, m_ctlVUPy);
	DDX_Control(pDX, IDC_SLIDER7, m_ctlVUPx);
	DDX_Control(pDX, IDC_SLIDER6, m_ctlVPNz);
	DDX_Control(pDX, IDC_SLIDER5, m_ctlVPNy);
	DDX_Control(pDX, IDC_SLIDER4, m_ctlVPNx);
	DDX_Control(pDX, IDC_SLIDER3, m_ctlCOPz);
	DDX_Control(pDX, IDC_SLIDER2, m_ctlCOPy);
	DDX_Control(pDX, IDC_SLIDER1, m_ctlCOPx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetView, CFormView)
	//{{AFX_MSG_MAP(CSetView)
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_COMMAND(ID_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetView diagnostics

#ifdef _DEBUG
void CSetView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSetView message handlers

void CSetView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("Pos is %d\n",nPos);
	CString str;
	int  ctlID;
	CMainFrame* pMain;
	CChildView* pChild;
	pMain=static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pChild=static_cast<CChildView*>(pMain->m_splitterwnd.GetPane(0,1));
//  这里有不解得问题
	if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CScrollBar ) ))
    {
        CFormView::OnHScroll( nSBCode, nPos, pScrollBar );
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSliderCtrl ) ))
    {
       	ctlID=pScrollBar->GetDlgCtrlID();
		if(nSBCode == SB_THUMBTRACK)
		{
			str.Format("%d",nPos);
			switch(ctlID)
			{
				case IDC_SLIDER1:
					GetDlgItem(IDC_STATICX)->SetWindowText(str);
//		m_ctlCOPx.SetPos(nPos);
					pChild->SetCOP(m_ctlCOPx.GetPos(),m_ctlCOPy.GetPos(),m_ctlCOPz.GetPos());
					break;
				case IDC_SLIDER2:
					GetDlgItem(IDC_STATICY)->SetWindowText(str);
					pChild->SetCOP(m_ctlCOPx.GetPos(),m_ctlCOPy.GetPos(),m_ctlCOPz.GetPos());
					break;
				case IDC_SLIDER3:
					GetDlgItem(IDC_STATICZ)->SetWindowText(str);
					pChild->SetCOP(m_ctlCOPx.GetPos(),m_ctlCOPy.GetPos(),m_ctlCOPz.GetPos());
					break;
				case IDC_SLIDER4:
					GetDlgItem(IDC_STATICX2)->SetWindowText(str);
					pChild->SetVPN(m_ctlVPNx.GetPos(),m_ctlVPNy.GetPos(),m_ctlVPNz.GetPos());
					break;
				case IDC_SLIDER5:
					GetDlgItem(IDC_STATICY2)->SetWindowText(str);
					pChild->SetVPN(m_ctlVPNx.GetPos(),m_ctlVPNy.GetPos(),m_ctlVPNz.GetPos());
					break;
				case IDC_SLIDER6:
					GetDlgItem(IDC_STATICZ2)->SetWindowText(str);
					pChild->SetVPN(m_ctlVPNx.GetPos(),m_ctlVPNy.GetPos(),m_ctlVPNz.GetPos());
					break;
				case IDC_SLIDER7:
					GetDlgItem(IDC_STATICX3)->SetWindowText(str);
					pChild->SetVUP(m_ctlVUPx.GetPos(),m_ctlVUPy.GetPos(),m_ctlVUPz.GetPos());
					break;
				case IDC_SLIDER8:
					GetDlgItem(IDC_STATICY3)->SetWindowText(str);
					pChild->SetVUP(m_ctlVUPx.GetPos(),m_ctlVUPy.GetPos(),m_ctlVUPz.GetPos());
					break;
				case IDC_SLIDER9:
					GetDlgItem(IDC_STATICZ3)->SetWindowText(str);
					pChild->SetVUP(m_ctlVUPx.GetPos(),m_ctlVUPy.GetPos(),m_ctlVUPz.GetPos());
					break;
				default:
					ASSERT(FALSE);
			}
			pChild->Draw();
		}
    }
    else if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CSpinButtonCtrl ) ))
    {
        CWnd::OnHScroll( nSBCode, nPos, pScrollBar );
    }

//	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSetView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ctlCOPx.SetRange(-100,100);
	m_ctlCOPx.SetPos(0);
	m_ctlCOPy.SetRange(-100,100);
	m_ctlCOPy.SetPos(0);
	m_ctlCOPz.SetRange(-74,74);
	m_ctlCOPz.SetPos(-10);
	m_ctlVPNx.SetRange(-20,20);
	m_ctlVPNx.SetPos(0);
	m_ctlVPNy.SetRange(-20,20);
	m_ctlVPNy.SetPos(0);
	m_ctlVPNz.SetRange(-20,20);
	m_ctlVPNz.SetPos(-1);
	m_ctlVUPx.SetRange(-20,20);
	m_ctlVUPx.SetPos(0);
	m_ctlVUPy.SetRange(-20,20);
	m_ctlVUPy.SetPos(1);
	m_ctlVUPz.SetRange(-20,20);
	m_ctlVUPz.SetPos(0);
}





void CSetView::OnSize(UINT nType, int cx, int cy) 
{

//	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CSetView::OnReset() 
{
	// TODO: Add your command handler code here
	m_ctlCOPx.SetPos(0);
	m_ctlCOPy.SetPos(0);
	m_ctlCOPz.SetPos(-10);
	m_ctlVPNx.SetPos(0);
	m_ctlVPNy.SetPos(0);
	m_ctlVPNz.SetPos(-1);
	m_ctlVUPx.SetPos(0);
	m_ctlVUPy.SetPos(1);
	m_ctlVUPz.SetPos(0);

	CMainFrame* pMain;
	CChildView* pChild;
	pMain=static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pChild=static_cast<CChildView*>(pMain->m_splitterwnd.GetPane(0,1));
	pChild->SetCOP(0,0,-10);
	pChild->SetVPN(0,0,-1);
	pChild->SetVUP(0,1,0);
	pChild->SetVRP(0,0,800);
	pChild->Draw();
}




