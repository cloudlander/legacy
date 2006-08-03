// GpaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GPA2.h"
#include "GpaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "GpaCore.h"
#include "Utility.h"

/////////////////////////////////////////////////////////////////////////////
// CGpaDlg dialog
extern unsigned __stdcall DoQueryGPA(void* arg);

extern CGpaCore g_gc;

CGpaDlg::CGpaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGpaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGpaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pTipWnd = NULL;
}


void CGpaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpaDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGpaDlg, CDialog)
	//{{AFX_MSG_MAP(CGpaDlg)
	ON_BN_CLICKED(ID_OK, OnOk)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGpaDlg message handlers

BOOL CGpaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
		RECT rc;
		GetWindowRect(&rc);
		MoveWindow(xCenter-(rc.right-rc.left)/2,yCenter-(rc.bottom-rc.top)/2,
			rc.right-rc.left,rc.bottom-rc.top);
		::SetFocus(::GetDlgItem(this->m_hWnd,IDC_STUDENTNO));
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



/////////////////////////////////////////////////////////////////////////////
// CGpaDlg




void CGpaDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
		TCHAR szBuf[100];
		::GetWindowText(GetDlgItem(IDC_STUDENTNO)->m_hWnd,szBuf,100);
		g_gc.m_StudentNo = szBuf;
		::GetWindowText(GetDlgItem(IDC_PASSWD)->m_hWnd,szBuf,100);
		g_gc.m_Passwd = szBuf;

		EndDialog(0);


}


void CGpaDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}
