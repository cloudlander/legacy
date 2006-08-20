// FTPClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTPClient.h"
#include "FTPClientDlg.h"
#include "SystemTray.h"		// 系统栏
#include "MD5Checksum.h"	// MD5散列
#include <vector>			// 链表	
#include <stack>			// 栈
#include <algorithm>		// 排序

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
class CFtpFileInfo;

UINT Refresh(LPVOID);	
void GetLocalIP(void);
void LoadRegKey(void);
void SaveRegKey(void);
void MakeLogFile(void);
BOOL UpLoadDS(vector<CFtpFileInfo>*,BOOL);
BOOL AuthIP(void);

class CFtpFileInfo
{
public:
	CString file;			// 文件或目录名
	CString UpDir;			// 上级目录的绝对路径
	BOOL	Directory;		// 是否目录
	long	FileBytes;		// 文件或目录大小
	CString	ModifiedTime;	// 文件创建时间

	// 顺序定义为文件的全路径的排序
	BOOL operator < (const CFtpFileInfo &Y) { return (UpDir + file < Y.UpDir + Y.file); }
	BOOL operator > (const CFtpFileInfo &Y) { return (UpDir + file > Y.UpDir + Y.file); }
	BOOL operator <=(const CFtpFileInfo &Y) { return (UpDir + file <=Y.UpDir + Y.file); }
	BOOL operator >=(const CFtpFileInfo &Y) { return (UpDir + file >=Y.UpDir + Y.file); }
	BOOL operator !=(const CFtpFileInfo &Y) { return (UpDir + file !=Y.UpDir + Y.file); }
	BOOL operator ==(const CFtpFileInfo &Y) { return (UpDir + file ==Y.UpDir + Y.file); }
};

// listAppend和listRemove均按顺序存储了
// 当前新加入的文件和删去的文件
vector<CFtpFileInfo>listAppend, listRemove;
BOOL bRefreshing;				// 当前是否处于工作状态
CFTPClientDlg *dlg;				// 方便对话框数据的访问
const char* kpcTrayNotificationMsg_ = "tray notification";
HKEY hk;
CString MD5Check;
BOOL bFtp, bDS, bAuth;

// 未知错误全局变量
CString g_DSError;
CString g_LocalIP;
// 数据库连接初始化
_bstr_t g_strCnn("Provider=sqloledb;Data Source=10.62.0.11;"
 		  "Initial Catalog=ftpUnion;User Id=ftpUnion;Password=qpyxzt;");
 
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

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
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPClientDlg dialog

CFTPClientDlg::CFTPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPClientDlg::IDD, pParent),
	bMinimized_(false),
	pTrayIcon_(0),
	nTrayNotificationMsg_(RegisterWindowMessage(kpcTrayNotificationMsg_))
{
	//{{AFX_DATA_INIT(CFTPClientDlg)
	m_AuthPwd = _T("");
	m_AuthUsr = _T("");
	m_FtpPwd = _T("");
	m_FtpUsr = _T("");
	m_Interval = 1;
	m_Path = _T("");
	m_NeedLog = TRUE;
	m_NeedLoop = TRUE;
	m_NeedUser = FALSE;
	m_Port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dlg=this;
	m_bFirstRun=FALSE;
}

void CFTPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPClientDlg)
	DDX_Control(pDX, IDC_FTP, m_IPFtp);
	DDX_Control(pDX, IDC_AUTHSVR, m_IPAuthSvr);
	DDX_Text(pDX, IDC_AUTHPWD, m_AuthPwd);
	DDX_Text(pDX, IDC_AUTHUSR, m_AuthUsr);
	DDX_Text(pDX, IDC_FTPPWD, m_FtpPwd);
	DDX_Text(pDX, IDC_FTPUSR, m_FtpUsr);
	DDX_Text(pDX, IDC_INTERVAL, m_Interval);
	DDV_MinMaxUInt(pDX, m_Interval, 1, 99999999);
	DDX_Text(pDX, IDC_PATH, m_Path);
	DDX_Check(pDX, IDC_NEEDLOG, m_NeedLog);
	DDX_Check(pDX, IDC_NEEDLOOP, m_NeedLoop);
	DDX_Check(pDX, IDC_NEEDUSER, m_NeedUser);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 1, 65535);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialog)
	//{{AFX_MSG_MAP(CFTPClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(IDR_RESTORE, OnSTRestore)
	ON_COMMAND(IDR_EXIT, OnSTExit)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_NEEDUSER, OnNeeduser)
	ON_BN_CLICKED(IDC_NEEDLOG, OnNeedlog)
	ON_BN_CLICKED(IDC_NEEDLOOP, OnNeedloop)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_AUTHENTICATE, OnAuthenticate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPClientDlg message handlers
void CFTPClientDlg::SetupTrayIcon()
{
	if (bMinimized_ && (pTrayIcon_ == 0)) 
	{
		pTrayIcon_ = new CSystemTray;
		pTrayIcon_->Create( 0, nTrayNotificationMsg_, 
							"复旦FTP联盟客户端更新程序",
							m_hIcon, IDR_SYSTRAY_MENU);
	}
	else 
	{
		delete pTrayIcon_;
		pTrayIcon_ = 0;
	}
}

void CFTPClientDlg::SetupTaskBarButton()
{
	// Show or hide this window appropriately
	if (bMinimized_) 
		ShowWindow(SW_HIDE);
	else
		ShowWindow(SW_SHOW);
}

BOOL CFTPClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (::AfxGetApp()->m_nCmdShow & SW_MINIMIZE) 
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}

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

	CStdioFile ftpOld;
	if (!ftpOld.Open("ftpOld.txt", CFile::modeRead))
	{
		ftpOld.Open("ftpOld.txt", CFile::modeCreate);
		m_bFirstRun=TRUE;
	}
	ftpOld.Close();

	GetLocalIP();
	LoadRegKey();
	UpdateData(FALSE);
	OnNeeduser();
	OnNeedlog();
	OnNeedloop();
	bRefreshing = FALSE;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFTPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		// Decide if minimize state changed
		bool bOldMin = bMinimized_;
		if (nID == SC_MINIMIZE) 
		{
			bMinimized_ = true;
		}
		else if (nID == SC_RESTORE) 
		{
			bMinimized_ = false;
		}

		CDialog::OnSysCommand(nID, lParam);

		if (bOldMin != bMinimized_) 
		{
			SetupTrayIcon();
			SetupTaskBarButton();
		}
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTPClientDlg::OnPaint() 
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
HCURSOR CFTPClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT Refresh(LPVOID lpParam)
{
	dlg->GetDlgItem(IDC_START)->EnableWindow(FALSE);
	CStatic *status=(CStatic *)dlg->GetDlgItem(IDC_STATUS);

	g_DSError="";
	dlg->GetDlgItem(IDC_AUTHENTICATE)->EnableWindow(FALSE);
	CoInitialize(NULL);

	if( !AuthIP() )                                                                                                        
	{                                                                                                                      
		 MakeLogFile();                                                                                                        
		 dlg->GetDlgItem(IDC_START)->EnableWindow(TRUE);                                                                       
		 dlg->GetDlgItem(IDC_AUTHENTICATE)->EnableWindow(TRUE);
		 dlg->SetDlgItemText(IDC_START, "开始");                                                                               
		 bRefreshing=FALSE;                                                                                                    
		 dlg->SetDlgItemText(IDC_STATUS, "Reday");    
		 CoUninitialize();
		 return 0;                                                                                                             
	}

	dlg->m_bFirstRun=TRUE;

	DWORD cbdata;
	RegQueryValueEx(hk, "bFirstRun", NULL, NULL, (LPBYTE)&dlg->m_bFirstRun, &cbdata);
	BOOL tempVal=TRUE;
	RegSetValueEx(hk, "bFirstRun", NULL, REG_DWORD, (LPBYTE)&tempVal, sizeof(DWORD));

	if(dlg->m_bFirstRun)
		::DeleteFile("ftpOld.txt");

	vector<CFtpFileInfo>listNew, listOld;
	int	i, iEnd;
	int u,j;
	int nTemp;


	listAppend.clear();
	listRemove.clear();

	// FTP会话
	CInternetSession sess("FTPClient");
	CFtpConnection* pConnect = NULL;

	// 获取用户输入的FTP地址
	struct in_addr target;
	dlg->m_IPFtp.GetAddress(target.S_un.S_addr);	
	target.S_un.S_addr=ntohl(target.S_un.S_addr);
	CString ftp=inet_ntoa((target));

	try
	{
		if (!dlg->m_NeedUser)
			pConnect = sess.GetFtpConnection(ftp, NULL, NULL, dlg->m_Port);
		else
			pConnect = sess.GetFtpConnection(ftp, dlg->m_FtpUsr, dlg->m_FtpPwd, dlg->m_Port);

		// 深度优先遍历整个FTP
		stack<CString> s;
		CString Dir,nextDir;
		pConnect->GetCurrentDirectory(Dir);
		s.push(Dir);
	
		BOOL bWorking;
		while (!s.empty())
		{
			nextDir = s.top();
			s.pop();
			if (!pConnect->SetCurrentDirectory(nextDir))
				continue;
			CFtpFileFind finder(pConnect);
			bWorking = finder.FindFile("*");
				
			while (bWorking)
			{
				bWorking = finder.FindNextFile();
				CString fs=finder.GetFilePath();
				dlg->SetDlgItemText(IDC_STATUS, fs);
				if (finder.IsDots())
					continue;

				CFtpFileInfo ffi;
				ffi.file=finder.GetFileName();
				nTemp=ffi.file.GetLength();
				u=-1;j=0;
				while(1){
					u=ffi.file.Find("'",j);
					if(u==-1)
						break;
					ffi.file.Insert(u,"'");
					j=u+2;
				}	
					
				int len=fs.GetLength() - nTemp;
				ffi.UpDir=fs.Left(len);

				u=-1;j=0;
				while(1){
					u=ffi.UpDir.Find("'",j);
					if(u==-1)
						break;
					ffi.UpDir.Insert(u,"'");
					j=u+2;
				}	

				ffi.Directory=finder.IsDirectory();
				ffi.FileBytes=ffi.Directory ? 0 : finder.GetLength();
				CTime t;
				finder.GetLastWriteTime(t);
				ffi.ModifiedTime=t.Format("%Y-%m-%d %H:%M:%S");
				listNew.push_back(ffi);

				if (finder.IsDirectory())
					s.push(fs+'/');
			}
			finder.Close();
		}
	}
	catch (CInternetException* pEx)
	{
		pEx->Delete();
		bFtp=FALSE;
		MakeLogFile();
		dlg->GetDlgItem(IDC_START)->EnableWindow(TRUE);
        dlg->GetDlgItem(IDC_AUTHENTICATE)->EnableWindow(TRUE);
		dlg->SetDlgItemText(IDC_START, "开始");
		bRefreshing=FALSE;
		dlg->SetDlgItemText(IDC_STATUS, "Reday");
		return 0;
	}
	bFtp=TRUE;
		
	if (pConnect != NULL)
		pConnect->Close();
	delete pConnect;
	
	// 对listNew排序
	sort(listNew.begin(), listNew.end());
	
	// 打开ftpOld文件
	CStdioFile ftpOld;

	if (ftpOld.Open("ftpOld.txt", CFile::modeRead))
	{
		CString str;
		while (ftpOld.ReadString(str))
		{
			CFtpFileInfo ffi;
			int m=0,n=0;
			
			n=str.Find('\t');
			ffi.file=str.Left(n);
			m=++n;
			
			n=str.Find('\t', n);
			ffi.UpDir=str.Mid(m,n-m);
			m=++n;

			n=str.Find('\t', n);
			ffi.Directory=atoi(str.Mid(m,n-m));
			m=++n;

			n=str.Find('\t', n);
			ffi.FileBytes=atoi(str.Mid(m,n-m));
			m=++n;

			ffi.ModifiedTime=str.Right(str.GetLength()-m);

			listOld.push_back(ffi);
		}
	}
	else
	{
		ftpOld.Open("ftpOld.txt", CFile::modeCreate);
	}
	ftpOld.Close();

	int iListNewLast=listNew.size();
	int iListOldLast=listOld.size();

	if (iListNewLast == 0 && iListOldLast ==0)
	{
	}
	else if ((iListNewLast == 0 && iListOldLast !=0))
	{
		for (i=0; i<iListOldLast; i++)
			listRemove.push_back(listOld[i]);
	}
	else if ((iListNewLast != 0 && iListOldLast ==0))
	{
		for (i=0; i<iListNewLast; i++)
			listAppend.push_back(listNew[i]);
	}
	else
	{
		iEnd = 0;
		for (i=0; i<iListNewLast; i++)
		{
			while (listOld[iEnd] < listNew[i] && iEnd != iListOldLast-1) 
				iEnd++;
			if (listOld[iEnd] != listNew[i])
				listAppend.push_back(listNew[i]);
		}

		iEnd = 0;
		for (i=0; i<iListOldLast; i++)
		{
			while (listNew[iEnd] < listOld[i] && iEnd != iListNewLast-1) 
				iEnd++;
			if (listNew[iEnd] != listOld[i])
				listRemove.push_back(listOld[i]);
		}
	}

/*	
	out.Open("ftpAppend.txt", CFile::modeWrite | CFile::modeCreate);
	for (i=0; i<listAppend.size(); i++)
		out.WriteString(listAppend[i].file+'\n');
	out.Close();

	out.Open("ftpRemove.txt", CFile::modeWrite | CFile::modeCreate);
	for (i=0; i<listRemove.size(); i++)
		out.WriteString(listRemove[i].file+'\n');
	out.Close();
*/
//  Modified here, make sure it's right for first run
	bDS=UpLoadDS(&listNew,dlg->m_bFirstRun);

	CoUninitialize();

	MakeLogFile();
    dlg->GetDlgItem(IDC_AUTHENTICATE)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_START)->EnableWindow(TRUE);
	dlg->SetDlgItemText(IDC_STATUS, "Reday");
	if (!dlg->m_NeedLoop)
	{
		dlg->SetDlgItemText(IDC_START, "开始");
		bRefreshing=!bRefreshing;
	}
	
	if (!bDS) 
	{
		::DeleteFile("ftpOld.txt");
		return 1;
	}
	CStdioFile out;
	out.Open("ftpOld.txt", CFile::modeWrite | CFile::modeCreate);
	for (i=0; i<iListNewLast; i++)
	{
		CString str,temp;
	
		str+=listNew[i].file+'\t';
		str+=listNew[i].UpDir+'\t';
		temp.Format("%d\t%d\t", listNew[i].Directory, listNew[i].FileBytes);
		str+=temp;
		str+=listNew[i].ModifiedTime+'\n';

		out.WriteString(str);
	}
	out.Close();
	
	CFile file("ftpOld.txt", CFile::modeRead);
	MD5Check=CMD5Checksum::GetMD5(file);
	file.Close();


	dlg->m_bFirstRun=FALSE;
	RegSetValueEx(hk, "bFirstRun", NULL, REG_DWORD, (LPBYTE)&dlg->m_bFirstRun, sizeof(DWORD));

	return 0;
}

void CFTPClientDlg::OnSTRestore() 
{
	ShowWindow(SW_RESTORE);
	bMinimized_ = false;
	SetupTrayIcon();
	SetupTaskBarButton();
}

void CFTPClientDlg::OnSTExit() 
{
	listAppend.clear();
	listRemove.clear();
	UpdateData();

	OnCancel();
}

BOOL CFTPClientDlg::DestroyWindow() 
{
	// Get rid of systray icon
	bMinimized_ = false;
	SetupTrayIcon();
	UpdateData();
	SaveRegKey();
	RegCloseKey(hk);	
	return CDialog::DestroyWindow();
}

void CFTPClientDlg::OnTimer(UINT nIDEvent) 
{
	CFile ftpOld;
	if (!ftpOld.Open("ftpOld.txt", CFile::modeRead))
		ftpOld.Open("ftpOld.txt", CFile::modeCreate);
	
	CString tmp=CMD5Checksum::GetMD5(ftpOld);
	if (tmp != MD5Check)
	{
		ftpOld.Close();
		ftpOld.Open("ftpOld.txt", CFile::modeCreate);
		ftpOld.Close();
		m_bFirstRun = TRUE;
	}

	th=AfxBeginThread(Refresh, NULL);
	CDialog::OnTimer(nIDEvent);
}

void CFTPClientDlg::OnStart() 
{
	if (!UpdateData(TRUE)) return;

	if (!bRefreshing)
	{
		SetDlgItemText(IDC_START, "停止");
		if (m_NeedLoop)
			SetTimer(1, m_Interval*3600*1000,NULL);
		OnTimer(1);
	}
	else
	{
		SetDlgItemText(IDC_START, "开始");
		KillTimer(1);	
	}
	bRefreshing=!bRefreshing;
}

void MakeLogFile()
{
	CStdioFile log;
	log.Open(dlg->m_Path, CFile::modeCreate | 
	                    CFile::modeNoTruncate |
		                CFile::modeWrite);

	CTime now=CTime::GetCurrentTime();
	CString str=now.Format("%c");
	if (!bAuth)
	{
		str+="\t客户端IP未授权\n"+g_DSError;
		goto end;
	}
	if (!bFtp)
	{
		str+="\tFTP连接失败\n";
		goto end;
	}
	if (!bDS)
	{
		str+="\t上传服务器失败\n"+g_DSError;
		goto end;
	}
	
	str+="\t任务成功完成\n";

end:
	log.SeekToEnd();
	log.WriteString(str);
	log.Close();
	dlg->SetDlgItemText(IDC_STATUS, str);
	::Sleep(5000);
	dlg->SetDlgItemText(IDC_STATUS, "Reday");
}

void CFTPClientDlg::OnNeeduser() 
{
	UpdateData();
	if (m_NeedUser)
	{
		GetDlgItem(IDC_FTPUSR)->EnableWindow(TRUE);
		GetDlgItem(IDC_FTPPWD)->EnableWindow(TRUE);
		GetDlgItem(IDC_FTPUSR)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_FTPUSR)->EnableWindow(FALSE);
		GetDlgItem(IDC_FTPPWD)->EnableWindow(FALSE);
	}
}

void CFTPClientDlg::OnNeedlog() 
{
	UpdateData();
	if (m_NeedLog)
		GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
}

void CFTPClientDlg::OnNeedloop() 
{
	UpdateData();
	if (m_NeedLoop)
	{
		GetDlgItem(IDC_INTERVAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_INTERVAL)->SetFocus();
	}
	else
		GetDlgItem(IDC_INTERVAL)->EnableWindow(FALSE);
}

void CFTPClientDlg::OnBrowse() 
{
    char szFilters[]= "日志文件 (*.log)|*.log|";

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog Dlg (TRUE, "log", "*.log",
	   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, this);
  
    if(Dlg.DoModal() == IDOK)
		m_Path = Dlg.GetPathName();

	UpdateData(FALSE);
}

void GetLocalIP(void)
{
	// 获取本机IP地址作为缺省的FTP地址
	WSADATA wsd;
	WSAStartup(MAKEWORD(1, 1), &wsd);

	char szHostName[128];
	if(gethostname(szHostName, 128) == 0)
	{
		// Get host adresses
		struct hostent *pHost;
		int i,j;
 
		pHost = gethostbyname(szHostName);
 
		for(i=0; pHost != NULL && pHost->h_addr_list[i] != NULL; i++)
		{
			CString str;
 			 
 			for(j = 0; j < pHost->h_length; j++)
 			{
				CString addr;
				
				if (j > 0)
					str+=".";

 				addr.Format("%u", (unsigned int)((unsigned char*)pHost->h_addr_list[i])[j]);
 				str+=addr;
			}
  			// str now contains one local IP address
			// but we only show the 10.***.***.*** adress
			if (str.Left(3) == "10."){
				dlg->m_IPFtp.SetAddress(ntohl(inet_addr(str)));
				g_LocalIP=str;
			}
 		}
	}
}

void LoadRegKey(void)
{
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Fudan FTP Union",
		0, "", 0, KEY_ALL_ACCESS, NULL,	&hk, NULL);
	
	DWORD cbData;
	BYTE *szUser, *szPath, *szCheck;
	dlg->m_Port=21;
	dlg->m_NeedUser=FALSE;
	dlg->m_NeedLog=TRUE;
	dlg->m_NeedLoop=TRUE;
	dlg->m_Interval=3;

	RegQueryValueEx(hk, "Port", NULL, NULL, (LPBYTE)&dlg->m_Port, &cbData);
	RegQueryValueEx(hk, "Anonymous", NULL, NULL, (LPBYTE)&dlg->m_NeedUser, &cbData);
	RegQueryValueEx(hk, "LogOn", NULL, NULL, (LPBYTE)&dlg->m_NeedLog, &cbData);
	RegQueryValueEx(hk, "AutoRefresh", NULL, NULL, (LPBYTE)&dlg->m_NeedLoop, &cbData);
	RegQueryValueEx(hk, "Interval", NULL, NULL, (LPBYTE)&dlg->m_Interval, &cbData);
	
	if (RegQueryValueEx(hk, "FtpUser", NULL, NULL, NULL, &cbData) == ERROR_SUCCESS)
	{
		if (cbData==0)
			dlg->m_FtpUsr="";
		else
		{
			szUser=new BYTE[cbData];
			RegQueryValueEx(hk, "FtpUser", NULL, NULL, szUser, &cbData);
			dlg->m_FtpUsr=szUser;
		}
	}
	else
		dlg->m_FtpUsr="";

	if (RegQueryValueEx(hk, "LogPath", NULL, NULL, NULL, &cbData) == ERROR_SUCCESS)
	{
		if (cbData==0)
			dlg->m_Path="save.log";
		else
		{
			szPath=new BYTE[cbData];
			RegQueryValueEx(hk, "LogPath", NULL, NULL, szPath, &cbData);
			dlg->m_Path=szPath;
		}
	}
	else
		dlg->m_Path="save.log";

	if (RegQueryValueEx(hk, "Validation", NULL, NULL, NULL, &cbData) == ERROR_SUCCESS)
	{
		if (cbData==0)
			MD5Check="";
		else
		{
			szCheck=new BYTE[cbData];
			RegQueryValueEx(hk, "Validation", NULL, NULL, szCheck, &cbData);
			MD5Check=szCheck;
		}
	}
	else
		MD5Check="";
}

void SaveRegKey(void)
{
	char szStr[255];

	RegSetValueEx(hk, "Port", NULL, REG_DWORD, (LPBYTE)&dlg->m_Port, sizeof(DWORD));
	RegSetValueEx(hk, "Anonymous", NULL, REG_DWORD, (LPBYTE)&dlg->m_NeedUser, sizeof(DWORD));
	lstrcpy(szStr, dlg->m_FtpUsr);
	RegSetValueEx(hk, "FtpUser", NULL, REG_SZ, (LPBYTE)szStr, lstrlen(szStr)+1);
	RegSetValueEx(hk, "LogOn", NULL, REG_DWORD, (LPBYTE)&dlg->m_NeedLog, sizeof(DWORD));
	lstrcpy(szStr, dlg->m_Path);
	RegSetValueEx(hk, "LogPath", NULL, REG_SZ, (LPBYTE)szStr, lstrlen(szStr)+1);
	RegSetValueEx(hk, "AutoRefresh", NULL, REG_DWORD, (LPBYTE)&dlg->m_NeedLoop, sizeof(DWORD));
	RegSetValueEx(hk, "Interval", NULL, REG_DWORD, (LPBYTE)&dlg->m_Interval, sizeof(DWORD));
	lstrcpy(szStr, MD5Check);
	RegSetValueEx(hk, "Validation", NULL, REG_SZ, (LPBYTE)szStr, lstrlen(szStr)+1);
	RegFlushKey(hk);
}

BOOL UpLoadDS(vector<CFtpFileInfo>* ListNew,BOOL bFirstRun)
{
	_ConnectionPtr pConnection = NULL;
	_RecordsetPtr pRS=NULL;
	_variant_t RowsAffected;

	_bstr_t temp;
	int nProgress;
	int iPro=0;
	double nTemp=0.0;
	CString strPro;
	CString str;
	CString FileName;
	struct in_addr target;
	dlg->m_IPFtp.GetAddress(target.S_un.S_addr);	
	target.S_un.S_addr=ntohl(target.S_un.S_addr);
	CString FtpIP=inet_ntoa((target));
	_bstr_t strUpdate;
	_bstr_t strAuthIP("select FileName from ServerList where IP = '" + FtpIP + "'" );

	int nListAppend=listAppend.size();
	int nListRemove=listRemove.size();

	int i;

	try
	{
      pConnection.CreateInstance(__uuidof(Connection));
      pConnection->Open(g_strCnn,"","",adConnectUnspecified);
	  TRACE("Connection established\n");
	  pRS=pConnection->Execute(strAuthIP,&RowsAffected,adCmdText);
	  if(!pRS->EndOfFile)
	  {
		  temp=pRS->Fields->GetItem("FileName")->Value;
/*
		  str=(char*)temp;
		  
		  int n=atoi(str.Right(3));
		  if(n<=500)
			  n+=500;
		  else
			  n-=500;
*/
          FileName=(char*)temp;;
		  dlg->SetDlgItemText(IDC_STATUS, "正在更新服务器");
		  nProgress = (nListAppend + nListRemove);

          // start updating database server
		  if(bFirstRun)
		  {
  			  strUpdate="delete from "+FileName;
			  pConnection->Execute(strUpdate,&RowsAffected,adCmdText);
/*
			  str2=(char*)strUpdate;
			  for(int j=0;j<str.GetLength();j++)
				  str2.SetAt(12+j,str[j]);
			  pConnection->Execute(str2.AllocSysString(),&RowsAffected,adCmdText);
*/
			  dlg->m_bFirstRun=FALSE;

		  }
		  for(i=0;i<nListAppend;i++)
		  {	
			  strUpdate="insert into "+FileName+" (FileName,UpDir,Directory,FileBytes,CreateTime) values (";
			  strUpdate=strUpdate+"\'"+listAppend[i].file+"\'"+","+"\'"+listAppend[i].UpDir+"\'"+",";

			  if(listAppend[i].Directory)
				  strUpdate+="1,";
			  else
				  strUpdate+="0,";
			  str.Format("%d",listAppend[i].FileBytes);
			  strUpdate=strUpdate+str+",";
			  strUpdate=strUpdate+"\'"+listAppend[i].ModifiedTime+"\'"+")";
			  TRACE(strUpdate);
			  pConnection->Execute(strUpdate,&RowsAffected,adCmdText);
			  iPro+=1;
			  strPro.Format("正在更新服务器...\t\t已完成%3d%%",iPro*100/nProgress);
			  if((double)iPro/nProgress - nTemp >0.01 ){
    			dlg->SetDlgItemText(IDC_STATUS, strPro);
				nTemp=(double)iPro/nProgress;
			  }

		  }

		  for(i=0;i<nListRemove;i++)
		  {		
			  strUpdate="delete from "+FileName+" where FileName ="+"\'";
			  strUpdate=strUpdate+listRemove[i].file+"\'"+" and UpDir=\'"+listRemove[i].UpDir+"\'";
			  TRACE(strUpdate);
			  pConnection->Execute(strUpdate,&RowsAffected,adCmdText);
		  			  iPro+=1;
			  strPro.Format("正在更新服务器...\t\t已完成%3d%%",iPro*100/nProgress);
			  if((double)iPro/nProgress - nTemp >0.01 ){
    			dlg->SetDlgItemText(IDC_STATUS, strPro);
				nTemp=(double)iPro/nProgress;
			  }

		  }

		  strUpdate="update ServerList set UpdateTime=getdate() , UpdateIP=\'" + g_LocalIP +"\',FileNum=(select count(*) from " +FileName+ ") where IP = '" + FtpIP + "'";
		  pConnection->Execute(strUpdate,&RowsAffected,adCmdText);

		  dlg->SetDlgItemText(IDC_STATUS, "服务器更新成功完成");

	  }
	  else
	  {
		  // Ftp IP not found in server, terminate process 
		  if(pRS!=NULL)  pRS->Close();
		  if(pConnection!=NULL)  pRS->Close();
		  return FALSE;
	  }
	  pRS->Close();
      pConnection->Close(); 
   }
 
   catch(_com_error &e)
   {
      _bstr_t bstrSource(e.Source());
      _bstr_t bstrDescription(e.Description());
	  CString strErr=(char*)strUpdate;
      g_DSError.Format("\t\t\t错误描述:Source : %s\n\t\t\t\t Description : %s \n\t\t\t\t Infected Command : %s\n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription,(LPCSTR)strErr);
	  AfxMessageBox("未知错误,请查看日志\n并与FTP联盟技术组联系");
	  return FALSE;
   }

	return TRUE;
}

void CFTPClientDlg::OnAuthenticate() 
{
	 _ConnectionPtr pConnection = NULL;
	 _RecordsetPtr pRS=NULL;
	 _variant_t RowsAffected;
//	 _bstr_t strCmd("update file001 set FileName='AAA' where FileName='abc.iso'");
	_bstr_t temp;
	CString str;
 	
	UpdateData();
	struct in_addr target;
	m_IPAuthSvr.GetAddress(target.S_un.S_addr);	
	target.S_un.S_addr=ntohl(target.S_un.S_addr);
	CString IPAllow=inet_ntoa((target));
	if(m_AuthUsr=="" || IPAllow=="0.0.0.0" || m_AuthPwd == "" || 0<=m_AuthPwd.Find("'",0))
    {
		AfxMessageBox("用户名，密码，IP地址不能为空\n且密码不可有字符 \'");
		return;
    }
		
	 _bstr_t strCmd("select * from ServerList where admin = \'" +
		             m_AuthUsr + "\' and password = \'" + m_AuthPwd + "\'");
	 _bstr_t strExc("update ServerList set IPAllowed = \'" + IPAllow + 
		             "\' where admin = \'" + m_AuthUsr + "\' and password =\'" + m_AuthPwd + "\'");

	 CoInitialize(NULL);
   
	 try
	{
      pConnection.CreateInstance(__uuidof(Connection));
	  SetDlgItemText(IDC_STATUS, "连接服务器...");
      pConnection->Open(g_strCnn,"","",adConnectUnspecified);
	  TRACE("Connection established\n");
	  SetDlgItemText(IDC_STATUS, "验证用户...");
	  pRS=pConnection->Execute(strCmd,&RowsAffected,adCmdText);
      SetDlgItemText(IDC_STATUS, "");
	  if(!pRS->EndOfFile){
			  pConnection->Execute(strExc,&RowsAffected,adCmdText);
			  AfxMessageBox("IP 授权成功完成");
	  }
	  else
		  AfxMessageBox("用户验证失败，请重试");
	  pRS->Close();
      pConnection->Close(); 
   }
 
   catch(_com_error &)
   {
//      _bstr_t bstrSource(e.Source());
//      _bstr_t bstrDescription(e.Description());
//	  CString strErr;

//    strErr.Format("\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);

	  AfxMessageBox("IP授权失败,请稍后重试\n或与FTP联盟技术组联系");

   }

	  CoUninitialize();
   	  m_AuthUsr="";
	  m_AuthPwd="";
	  m_IPAuthSvr.ClearAddress();
	  UpdateData(FALSE);

}

BOOL AuthIP()
{
	_ConnectionPtr pConnection = NULL;
	_RecordsetPtr pRS=NULL;
	_variant_t RowsAffected;
	struct in_addr target;
	dlg->m_IPFtp.GetAddress(target.S_un.S_addr);	
	target.S_un.S_addr=ntohl(target.S_un.S_addr);
	CString FtpIP=inet_ntoa((target));
	_bstr_t temp;
	_bstr_t strAuthIP("select FileName,IPAllowed from ServerList where IP = '" + FtpIP + "'" );

	try
	{
      pConnection.CreateInstance(__uuidof(Connection));
      dlg->SetDlgItemText(IDC_STATUS, "客户端验证...");
      pConnection->Open(g_strCnn,"","",adConnectUnspecified);
	  TRACE("Connection established\n");
	  pRS=pConnection->Execute(strAuthIP,&RowsAffected,adCmdText);
	  if(!pRS->EndOfFile)
	  {
		  temp=pRS->Fields->GetItem("IPAllowed")->Value;
		  
		  if(0 != g_LocalIP.Compare(temp))   // IP not allowed 
		  {
			  bAuth=FALSE;
    		  dlg->SetDlgItemText(IDC_STATUS, "未经授权的IP");
			  if(pRS!=NULL)  pRS->Close();
			  if(pConnection!=NULL)  pConnection->Close();
			  return FALSE;
		  }	
		  else{
			  bAuth=TRUE;
			  return TRUE;
		  }
		  
		  if(pRS!=NULL)  pRS->Close();
		  if(pConnection!=NULL)  pConnection->Close();
	  }
	}
	catch(_com_error &e)
	{
	      _bstr_t bstrSource(e.Source());
		  _bstr_t bstrDescription(e.Description());
		  g_DSError.Format("\t\t\t错误描述:Source : %s\n\t\t\t\t Description : %s\n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);

		  AfxMessageBox("未知错误,请查看日志\n并与FTP联盟技术组联系");

		  return FALSE;

	}

	return TRUE;
}







































































































































































