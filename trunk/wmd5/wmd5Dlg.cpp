// wmd5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wmd5.h"
#include "wmd5Dlg.h"
#include "cjhyperlink.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	CCJHyperLink	m_staticEmail;
	
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAILME, m_staticEmail);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWmd5Dlg dialog

CWmd5Dlg::CWmd5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWmd5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWmd5Dlg)
	m_JobPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
#ifdef SHOW_TIME_LEFT
	m_strTimeLeft = _T("");
#endif

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWmd5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWmd5Dlg)
	DDX_Control(pDX, IDC_SUCCESS, m_success);
	DDX_Control(pDX, IDC_ERROR, m_error);
	DDX_Control(pDX, IDC_NOTMATCH, m_notmatch);
	DDX_Control(pDX, IDC_MATCH, m_match);
	DDX_Control(pDX, IDC_QueueProgress, m_QueueProgress);
	DDX_Control(pDX, IDC_JOBLIST, m_JobList);
	DDX_Text(pDX, IDC_JOBPATH, m_JobPath);
	//}}AFX_DATA_MAP
#ifdef SHOW_TIME_LEFT
	DDX_Control(pDX, IDC_TIMELEFT, m_TimeLeft);
	DDX_Text(pDX, IDC_TIMELEFT, m_strTimeLeft);
#endif
}

BEGIN_MESSAGE_MAP(CWmd5Dlg, CDialog)
	//{{AFX_MSG_MAP(CWmd5Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_JOBLIST, OnClickJoblist)
	//}}AFX_MSG_MAP
#ifdef SHOW_TIME_LEFT
	ON_WM_TIMER()
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWmd5Dlg message handlers

BOOL CWmd5Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*  // test code 
	CJobProgresser* pcc=new CJobProgresser;
	pcc->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(10,10,200,30),
		GetDlgItem(IDC_TREADGROUP),1);
*/
	// initialize the listctrl
	CRect rc;
	m_JobList.GetClientRect(&rc);
	int i=0;
	LV_COLUMN lvc;
	m_JobList.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
    m_JobList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	lvc.mask=LVCF_WIDTH;
	lvc.cx=0;
	m_JobList.InsertColumn(i,&lvc);		// refer to MSDN, dummy column to make all column texts centered
	i++;
	lvc.pszText=_T("Result");
	lvc.mask=LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;
	lvc.fmt=LVCFMT_CENTER;
	lvc.cx=rc.right/2;
	m_JobList.InsertColumn(i,&lvc);
	i++;
	lvc.cx=rc.right/2;
	lvc.pszText=_T("File");
	m_JobList.InsertColumn(i,&lvc);
	m_JobList.DeleteColumn(0);


#ifdef SHOW_TIME_LEFT
	m_strTimeLeft=_T("0:0:0");
	UpdateData(FALSE);
	SetTimer(1,1000,NULL);
#endif
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWmd5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWmd5Dlg::OnPaint() 
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
HCURSOR CWmd5Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CWmd5Dlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CWmd5Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(DONE==m_pQueue->GetStatus())
	{
		m_pQueue->DoSaveResult();
		PostQuitMessage(0);
	}
	CDialog::OnClose();
}


/* user resizing is disable currently */
void CWmd5Dlg::OnSize(UINT nType, int cx, int cy) 
{	
/*
	if(GetDlgItem(IDC_TREADGROUP))
	{
		GetDlgItem(IDC_TREADGROUP)->MoveWindow(10,10,cx-10,cy-10);
		TRACE("GROUP %d,%d\n",cx-10,cy-10);
		GetDlgItem(IDC_TREADGROUP)->SendMessageToDescendants(WM_SIZE,SIZE_RESTORED,MAKELPARAM(cx-10,cy-10),FALSE,TRUE);
	}
*/
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

#ifdef SHOW_TIME_LEFT
void CWmd5Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData();
	unsigned int leftHours,leftMins,leftSecs;
	sscanf((LPCTSTR)m_strTimeLeft,"%d:%d:%d",&leftHours,&leftMins,&leftSecs);
	if(((CWmd5App*)AfxGetApp())->m_Queue.GetStatus() == DONE || ((CWmd5App*)AfxGetApp())->m_Queue.GetStatus() == FINISH
		||!leftHours && !leftMins && !leftSecs)
		return;
	leftSecs--;
	if(0>leftSecs)
	{	
		leftSecs=59;
		leftMins--;
		if(0>leftMins)
		{
			leftMins=59;
			leftHours--;
			if(leftHours<0)
			{
				leftHours=leftMins=leftSecs=0;
			}
		}
	}
	m_strTimeLeft.Format("%d:%d:%d",leftHours,leftMins,leftSecs);
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
#endif

void CWmd5Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_pQueue && RUNNING==m_pQueue->GetStatus())
	{
		m_pQueue->SuspendWork();
		if(IDYES==AfxMessageBox(_T("Cancel all jobs?"),MB_YESNO))
		{
			m_pQueue->SetStatus(CANCELED);
		}	
		m_pQueue->ResumeWork();
	}
//	CDialog::OnCancel();
}

void CWmd5Dlg::OnOK() 
{
	// TODO: Add extra cleanup here
	OnCancel();
}

void CWmd5Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
//	DWORD tmp=GetCurrentThreadId();
	PostQuitMessage(0);
	// TODO: Add your message handler code here
	
}

void CWmd5Dlg::OnClickJoblist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_JobList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
			int nItem = m_JobList.GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nItem);
			// you could do your own processing on nItem here
			m_JobPath.Format(_T("File Path -- %s"),(TCHAR*)m_JobList.GetItemData(nItem));
			UpdateData(FALSE);
	}
	
	*pResult = 0;
}

void CWmd5Dlg::ShowResult(CString total, CString error)
{
	m_QueueProgress.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SSUCCESS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SERROR)->ShowWindow(SW_SHOW);
	m_success.ShowWindow(SW_SHOW);
	m_error.ShowWindow(SW_SHOW);
	m_success.SetWindowText(total);
	m_error.SetWindowText(error);
	UpdateWindow();
}

void CWmd5Dlg::ShowResult(CString total, CString error, CString match, CString notmatch)
{
	m_QueueProgress.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SSUCCESS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SERROR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SMATCH)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SNOTMATCH)->ShowWindow(SW_SHOW);
	m_success.ShowWindow(SW_SHOW);
	m_error.ShowWindow(SW_SHOW);
	m_match.ShowWindow(SW_SHOW);
	m_notmatch.ShowWindow(SW_SHOW);
	m_success.SetWindowText(total);
	m_error.SetWindowText(error);
	m_match.SetWindowText(match);
	m_notmatch.SetWindowText(notmatch);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_staticEmail.SetURL(_T("mailto:zhangxiaomin@gmail.com"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
