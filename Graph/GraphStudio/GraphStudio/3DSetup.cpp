// 3DSetup.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "3DSetup.h"
#include "3DView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DSetup dialog


C3DSetup::C3DSetup(CWnd* pParent /*=NULL*/)
	: CDialog(C3DSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3DSetup)
	//}}AFX_DATA_INIT
}


void C3DSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DSetup)
	DDX_Control(pDX, IDC_SLIDER9, m_ctlVUPz);
	DDX_Control(pDX, IDC_SLIDER8, m_ctlVUPy);
	DDX_Control(pDX, IDC_SLIDER7, m_ctlVUPx);
	DDX_Control(pDX, IDC_SLIDER6, m_ctlVPNz);
	DDX_Control(pDX, IDC_SLIDER5, m_ctlVPNy);
	DDX_Control(pDX, IDC_SLIDER4, m_ctlVPNx);
	DDX_Control(pDX, IDC_SLIDER2, m_ctlCOPy);
	DDX_Control(pDX, IDC_SLIDER3, m_ctlCOPz);
	DDX_Control(pDX, IDC_SLIDER1, m_ctlCOPx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DSetup, CDialog)
	//{{AFX_MSG_MAP(C3DSetup)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DSetup message handlers

BOOL C3DSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DSetup::SetView(CView* pView)
{
	m_pView = pView;
}

void C3DSetup::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	int  ctlID;
//	CMainFrame* pMain;
	C3DView* pChild;
//	pMain=static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pChild=reinterpret_cast<C3DView*>(m_pView);
//  这里有不解得问题
	if ( pScrollBar->IsKindOf( RUNTIME_CLASS( CScrollBar ) ))
    {
        CDialog::OnHScroll( nSBCode, nPos, pScrollBar );
		return;
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
        CDialog::OnHScroll( nSBCode, nPos, pScrollBar );
		return;
    }	
//	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void C3DSetup::OnReset() 
{
	// TODO: Add your control notification handler code here
	C3DView* pChild;
	pChild=reinterpret_cast<C3DView*>(m_pView);
	m_ctlCOPx.SetPos(0);
	m_ctlCOPy.SetPos(0);
	m_ctlCOPz.SetPos(-10);
	m_ctlVPNx.SetPos(0);
	m_ctlVPNy.SetPos(0);
	m_ctlVPNz.SetPos(-1);
	m_ctlVUPx.SetPos(0);
	m_ctlVUPy.SetPos(1);
	m_ctlVUPz.SetPos(0);
	pChild->SetCOP(0,0,-10);
	pChild->SetVPN(0,0,-1);
	pChild->SetVUP(0,1,0);
	pChild->SetVRP(0,0,800);
	pChild->Draw();
	
}
