// installerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "installer.h"
#include "installerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstallerDlg dialog

CInstallerDlg::CInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstallerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstallerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInstallerDlg, CDialog)
	//{{AFX_MSG_MAP(CInstallerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_INSTALL, OnInstall)
	ON_BN_CLICKED(ID_UNINSTALL, OnUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstallerDlg message handlers

BOOL CInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInstallerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInstallerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
typedef HRESULT (*FUNC)();

void CInstallerDlg::OnInstall() 
{
	// TODO: Add your control notification handler code here
	FUNC RegFunc;
	HMODULE hm = LoadLibrary("iEnhance.dll");
	if(hm != NULL)
	{
		RegFunc = (FUNC)GetProcAddress(hm,"DllRegisterServer");
	}
	if(hm == NULL || RegFunc == 0)
	{
		AfxMessageBox("��װʧ��");
	}
	else
	{
		if(FAILED(RegFunc()))
		{
			AfxMessageBox("ע��ʧ��");
		}
		else
			AfxMessageBox("��װ�ɹ�\r\nлл����");
	}
}

void CInstallerDlg::OnUninstall() 
{
	// TODO: Add your control notification handler code here
	FUNC RegFunc;
	HMODULE hm = LoadLibrary("iEnhance.dll");
	if(hm != NULL)
	{
		RegFunc = (FUNC)GetProcAddress(hm,"DllUnregisterServer");
	}
	if(hm == NULL || RegFunc == 0)
	{
		AfxMessageBox("ж��ʧ��");
	}
	else
	{
		if(FAILED(RegFunc()))
		{
			AfxMessageBox("ж��ʧ��");
		}
		else
			AfxMessageBox("�ɹ�ж��\r\nлл����");
	}
	
}

void CInstallerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
