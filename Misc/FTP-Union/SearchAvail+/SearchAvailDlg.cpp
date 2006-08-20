// SearchAvailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchAvail.h"
#include "SearchAvailDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*   Found in WinInet.h
We Only care about 3 condition
1			means successful connection
The Others	means failure to connect in
 
#define INTERNET_ERROR_BASE                     12000

#define ERROR_INTERNET_OUT_OF_HANDLES           (INTERNET_ERROR_BASE + 1)
#define ERROR_INTERNET_TIMEOUT                  (INTERNET_ERROR_BASE + 2)
#define ERROR_INTERNET_EXTENDED_ERROR           (INTERNET_ERROR_BASE + 3)
#define ERROR_INTERNET_INTERNAL_ERROR           (INTERNET_ERROR_BASE + 4)
#define ERROR_INTERNET_INVALID_URL              (INTERNET_ERROR_BASE + 5)
#define ERROR_INTERNET_UNRECOGNIZED_SCHEME      (INTERNET_ERROR_BASE + 6)
#define ERROR_INTERNET_NAME_NOT_RESOLVED        (INTERNET_ERROR_BASE + 7)
#define ERROR_INTERNET_PROTOCOL_NOT_FOUND       (INTERNET_ERROR_BASE + 8)
#define ERROR_INTERNET_INVALID_OPTION           (INTERNET_ERROR_BASE + 9)
#define ERROR_INTERNET_BAD_OPTION_LENGTH        (INTERNET_ERROR_BASE + 10)
#define ERROR_INTERNET_OPTION_NOT_SETTABLE      (INTERNET_ERROR_BASE + 11)
#define ERROR_INTERNET_SHUTDOWN                 (INTERNET_ERROR_BASE + 12)
#define ERROR_INTERNET_INCORRECT_USER_NAME      (INTERNET_ERROR_BASE + 13)
#define ERROR_INTERNET_INCORRECT_PASSWORD       (INTERNET_ERROR_BASE + 14)
#define ERROR_INTERNET_LOGIN_FAILURE            (INTERNET_ERROR_BASE + 15)
#define ERROR_INTERNET_INVALID_OPERATION        (INTERNET_ERROR_BASE + 16)
#define ERROR_INTERNET_OPERATION_CANCELLED      (INTERNET_ERROR_BASE + 17)
#define ERROR_INTERNET_INCORRECT_HANDLE_TYPE    (INTERNET_ERROR_BASE + 18)
#define ERROR_INTERNET_INCORRECT_HANDLE_STATE   (INTERNET_ERROR_BASE + 19)
#define ERROR_INTERNET_NOT_PROXY_REQUEST        (INTERNET_ERROR_BASE + 20)
#define ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND (INTERNET_ERROR_BASE + 21)
#define ERROR_INTERNET_BAD_REGISTRY_PARAMETER   (INTERNET_ERROR_BASE + 22)
#define ERROR_INTERNET_NO_DIRECT_ACCESS         (INTERNET_ERROR_BASE + 23)
#define ERROR_INTERNET_NO_CONTEXT               (INTERNET_ERROR_BASE + 24)
#define ERROR_INTERNET_NO_CALLBACK              (INTERNET_ERROR_BASE + 25)
#define ERROR_INTERNET_REQUEST_PENDING          (INTERNET_ERROR_BASE + 26)
#define ERROR_INTERNET_INCORRECT_FORMAT         (INTERNET_ERROR_BASE + 27)
#define ERROR_INTERNET_ITEM_NOT_FOUND           (INTERNET_ERROR_BASE + 28)
#define ERROR_INTERNET_CANNOT_CONNECT           (INTERNET_ERROR_BASE + 29)
#define ERROR_INTERNET_CONNECTION_ABORTED       (INTERNET_ERROR_BASE + 30)
#define ERROR_INTERNET_CONNECTION_RESET         (INTERNET_ERROR_BASE + 31)
#define ERROR_INTERNET_FORCE_RETRY              (INTERNET_ERROR_BASE + 32)
#define ERROR_INTERNET_INVALID_PROXY_REQUEST    (INTERNET_ERROR_BASE + 33)
#define ERROR_INTERNET_NEED_UI                  (INTERNET_ERROR_BASE + 34)

*/

using namespace std;

typedef struct tagFTPStatus
{
	CString ftp;
	BOOL	bAvail;
	DWORD	ErrCode;
}FTPStatus;

vector<FTPStatus> v;
UINT VerifyFTPSite(LPVOID lpParam);

void UpdateServer(int ,CString&);
_bstr_t g_strCnn("Provider=sqloledb;Data Source=10.62.0.11;"
 		  "Initial Catalog=ftpUnion;User Id=ftpUnion;Password=qpyxzt;");

/////////////////////////////////////////////////////////////////////////////
// CSearchAvailDlg dialog

CSearchAvailDlg::CSearchAvailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchAvailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchAvailDlg)
	m_Interval = 120;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSearchAvailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchAvailDlg)
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Text(pDX, IDC_EDIT1, m_Interval);
	DDV_MinMaxUInt(pDX, m_Interval, 60, 60000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchAvailDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchAvailDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchAvailDlg message handlers

BOOL CSearchAvailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSearchAvailDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSearchAvailDlg::OnPaint() 
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
HCURSOR CSearchAvailDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 开启定时器
void CSearchAvailDlg::OnStart() 
{
	UpdateData(TRUE);
	SetTimer(1, m_Interval*1000, NULL);
	OnTimer(1);
}

// 关闭定时器
void CSearchAvailDlg::OnStop() 
{
	KillTimer(1);	
}

// 站点登陆
UINT VerifyFTPSite(LPVOID lpParam)
{
	// 列表中站点的索引
	int i=(int)lpParam;

	//打开Internet会话
	CInternetSession sess("FTPSearcher");
	CFtpConnection* pConnect = NULL;

	try
	{
		// 成功登陆
		pConnect = sess.GetFtpConnection(v[i].ftp);
		v[i].bAvail=TRUE;
		v[i].ErrCode=1;
	}
	catch (CInternetException* pEx)
	{
		// 失败登陆
		v[i].ErrCode=pEx->m_dwError;
		pEx->Delete();
	}
	
	if (pConnect != NULL)
		pConnect->Close();
	delete pConnect;

	return 0;
}

// 定时器处理
void CSearchAvailDlg::OnTimer(UINT nIDEvent) 
{
	// ftpList存储所有的ftp站点地址
	// ftpAvail存储当前可用的站点地址
	CStdioFile ftpList,ftpAvail;
	ftpList.Open("ftp.txt", CFile::modeRead);
	ftpAvail.Open("ftpavail.txt", CFile::modeCreate | 
	                    CFile::modeNoTruncate |
		                CFile::modeWrite);
	int i;
	CString ftp;
	CString Err;


	v.clear();
	// 读如所有的站点到列表中
	while (ftpList.ReadString(ftp))
	{
		FTPStatus temp;
		temp.ftp=ftp;
		temp.bAvail=FALSE;
		temp.ErrCode=ERROR_INTERNET_TIMEOUT;
		v.push_back(temp);
	}

	m_Status.SetWindowText("waiting...");
	// 对每个站点启动一个线程尝试登陆
	for (i=0; i<v.size(); i++)
		AfxBeginThread(VerifyFTPSite, (LPVOID)i);
		
	//阻塞60秒，超出60秒的均被认为是超时错误
	::Sleep(10000);
	// 收尾工作
    CoInitialize(NULL);
	ftpAvail.SeekToEnd();
	for (i=0; i<v.size(); i++)
	{
		CString temp;
		Err="";
		UpdateServer(i,Err);
		temp.Format("\t%d\t",v[i].ErrCode);
		ftpAvail.WriteString(v[i].ftp+temp+Err);
	}

	CoUninitialize();
	
	m_Status.SetWindowText("done");
	
	ftpList.Close();
	ftpAvail.Close();
	
	CDialog::OnTimer(nIDEvent);
}

void UpdateServer(int i,CString& log)
{
	 _ConnectionPtr pConnection = NULL;
	 _RecordsetPtr pRS=NULL;
	 _variant_t RowsAffected;
	 CString str;
	 if(v[i].ErrCode > 1)
 		str.Format("%d",0);
	 else
		 str.Format("%d",1);
	 _bstr_t strCmd("select * from ServerList where IP = \'" + v[i].ftp + "\'");
	 _bstr_t strExc("update ServerList set available = \'" + str + "\' " + "where IP = \'" + v[i].ftp + "\'");

   
	 try
	{
      pConnection.CreateInstance(__uuidof(Connection));
      pConnection->Open(g_strCnn,"","",adConnectUnspecified);
	  TRACE("Connection established\n");
	  pRS=pConnection->Execute(strCmd,&RowsAffected,adCmdText);
	  if(!pRS->EndOfFile){
			  pConnection->Execute(strExc,&RowsAffected,adCmdText);
			  log.Format("Update Successfully\n");	
	  }
	  else{
			  log.Format("Failed,FTP Server Not Found\n");
	  }
	  pRS->Close();
      pConnection->Close(); 
   }
 
   catch(_com_error &e)
   {
      _bstr_t bstrSource(e.Source());
      _bstr_t bstrDescription(e.Description());
      log.Format("Failed, Source:%s\tDescription:%s\n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
   }
}
