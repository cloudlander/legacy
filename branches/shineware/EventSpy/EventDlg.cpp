// EventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EventSpy.h"
#include "EventDlg.h"
#define IENH_EXT_EXPORTS
#include "../ext/ienh_ext.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class Global
{
public:
	CEventDlg* m_pDlg;
	HWND	m_hWndHost;
	HWND	m_hWndFrame;
	IWebBrowser2* m_pIEFrame;
	int m_Sess;
	Global()
	{
		m_pDlg=NULL;
	}
	~Global()
	{
		if(m_pDlg&&::IsWindow(m_pDlg->m_hWnd))
		{
			m_pDlg->DestroyWindow();
		}
	}
	void InsertString(CString s)
	{
		m_pDlg->InsertString(s);
	}
	void CloseSession()
	{
		::SendMessage(m_hWndHost,WM_IENHEXT_CLOSESESSION,m_Sess,0);
	}
};

Global g_Sess;
BOOL g_bMore=FALSE;
BOOL g_bCapAll=FALSE;

BOOL Create(HWND hwndHost,HWND hwndFrame,void* pFrameWB,int sess)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_Sess.m_hWndFrame=hwndFrame;
	g_Sess.m_hWndHost=hwndHost;
	g_Sess.m_pIEFrame=reinterpret_cast<IWebBrowser2*>(pFrameWB);
	g_Sess.m_Sess=sess;
	g_Sess.m_pDlg=new CEventDlg;
	g_Sess.m_pDlg->Create();
	g_Sess.m_pDlg->ShowWindow(SW_SHOW);
	return TRUE;
}

void OnBeforeNavigate2(IDispatch* pDisp,VARIANT* Url,VARIANT* Flags,
					   VARIANT* TargetFrameName,VARIANT* PostData,
					   VARIANT* Headers,VARIANT_BOOL* Cancel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Before Navigate ";
	if(g_bMore)
	{
		CString tmp;
		tmp="---URL:"+CString(Url->bstrVal);
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnCommandStateChange(long Command,VARIANT_BOOL Enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Command State Change ";
	if(g_bCapAll)
	{
		if(g_bMore)
		{
			CString tmp;
			tmp.Format("---Command:%d",Command);
			str+=tmp;
		}
		g_Sess.InsertString(str);	
	}
}

void OnDocumentComplete(IDispatch* pDisp,VARIANT* URL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Document Complete ";
	if(g_bMore)
	{
		CString tmp;
		tmp="---URL:"+CString(URL->bstrVal);
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnDownloadBegin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Download Begin";
	g_Sess.InsertString(str);	
}

void OnDownloadComplete()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Download Complete";
	g_Sess.InsertString(str);	
}

void OnNavigateComplete2(IDispatch* pDisp,VARIANT* URL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Navigate Complete";
	if(g_bMore)
	{
		CString tmp;
		tmp="---URL:"+CString(URL->bstrVal);
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="New Window";
	if(g_bMore)
	{
		CString tmp="---Canceled:";
		if(VARIANT_TRUE==*Cancel)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnFullScreen(VARIANT_BOOL FullScreen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Full Screen";
	if(g_bMore)
	{
		CString tmp="---Full Screen Mode:";
		if(VARIANT_TRUE==FullScreen)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnMenuBar(VARIANT_BOOL MenuBar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Menu Bar";
	if(g_bMore)
	{
		CString tmp="---Show Menu Bar:";
		if(VARIANT_TRUE==MenuBar)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnQuit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Quiting";
	g_Sess.InsertString(str);	
}

void OnStatusBar(VARIANT_BOOL StatusBar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Status Bar";
	if(g_bMore)
	{
		CString tmp="---Show Status Bar:";
		if(VARIANT_TRUE==StatusBar)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnTheaterMode(VARIANT_BOOL TheaterMode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Theater Mode";
	if(g_bMore)
	{
		CString tmp="---Theater Mode:";
		if(VARIANT_TRUE==TheaterMode)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnToolBar(VARIANT_BOOL ToolBar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Toolbar";
	if(g_bMore)
	{
		CString tmp="---Show Toolbar:";
		if(VARIANT_TRUE==ToolBar)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnVisible(VARIANT_BOOL Visible)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Visible";
	if(g_bMore)
	{
		CString tmp="---Now Visible:";
		if(VARIANT_TRUE==Visible)
			tmp+="Yes";
		else
			tmp+="No";
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnProgressChange(long Progress,long ProgressMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Progress Change";
	if(g_bCapAll)
	{
		if(g_bMore)
		{
			CString tmp;
			tmp.Format("---Progress: %d of %d",Progress,ProgressMax);
			str+=tmp;
		}
		g_Sess.InsertString(str);	
	}
}

void OnPropertyChange(BSTR szProperty)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Property Change";
	if(g_bMore)
	{
		CString tmp;
		tmp.Format("---Property %s Changing",CString(szProperty).GetBuffer(-1));
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

void OnStatusTextChange(BSTR Text)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Status Text Change";
	if(g_bCapAll)
	{
		if(g_bMore)
		{
			CString tmp;
			tmp.Format("---Status Changed to %s",CString(Text).GetBuffer(-1));
			str+=tmp;
		}
		g_Sess.InsertString(str);	
	}
}

void OnTitleChange(BSTR Text)				
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString str;
	str="Title Change";
	if(g_bMore)
	{
		CString tmp;
		tmp.Format("---Title Changed to %s",CString(Text).GetBuffer(-1));
		str+=tmp;
	}
	g_Sess.InsertString(str);	
}

/////////////////////////////////////////////////////////////////////////////
// CEventDlg dialog
CEventDlg::CEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventDlg::IDD, pParent)
	, m_bMore(FALSE)
	, m_bCapAll(FALSE)
{
	//{{AFX_DATA_INIT(CEventDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bMore);
	DDX_Check(pDX, IDC_CHECK2, m_bCapAll);
}


BEGIN_MESSAGE_MAP(CEventDlg, CDialog)
	//{{AFX_MSG_MAP(CEventDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventDlg message handlers

BOOL CEventDlg::Create() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, CWnd::FromHandle(g_Sess.m_hWndFrame));
}

void CEventDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_Sess.CloseSession();
	delete this;
	CDialog::PostNcDestroy();
}

void CEventDlg::InsertString(CString s)
{
	m_ListBox.AddString(s);
}

void CEventDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CEventDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow();
	m_bMore=g_bMore;
	m_bCapAll=g_bCapAll;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEventDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_ListBox.ResetContent();
}

void CEventDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	g_bMore=m_bMore;
}

void CEventDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CEventDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	g_bCapAll=m_bCapAll;
}

void CEventDlg::OnLbnDblclkList1()
{
	// TODO: Add your control notification handler code here
	CString tmp;
	int i=m_ListBox.GetCurSel();
	m_ListBox.GetText(i,tmp);
	AfxMessageBox(tmp);
}
