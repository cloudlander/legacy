// wmd5.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "wmd5.h"
#include "wmd5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "workqueue.h"

/////////////////////////////////////////////////////////////////////////////
// CWmd5App

BEGIN_MESSAGE_MAP(CWmd5App, CWinApp)
	//{{AFX_MSG_MAP(CWmd5App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWmd5App construction

CWmd5App::CWmd5App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWmd5App object
struct queue_param{
	MyCommandLineInfo* cmdinfo;
	CWmd5Dlg* dlg;
};

CWmd5App theApp;
UINT QueueThread(LPVOID param)
{
	struct queue_param* p=(queue_param*)param;
	CWorkQueue* pQueue=new CWorkQueue;
	pQueue->Initialize(p->cmdinfo,p->dlg);
	pQueue->Run();
	pQueue->Wait();
	pQueue->Update();
	if(CANCELED==pQueue->GetStatus())		// let GUI do close thing if DONE normally
		pQueue->m_pDlg->SendMessage(WM_DESTROY);
	else
		pQueue->ShowResult();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CWmd5App initialization

BOOL CWmd5App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	InitCommonControls();

	CWmd5Dlg* pdlg=new CWmd5Dlg;
	m_pMainWnd=pdlg;
	pdlg->Create(NULL);
	pdlg->ShowWindow(SW_SHOW);
	
	MyCommandLineInfo* cmdinfo=new MyCommandLineInfo;
	ParseCommandLine(*cmdinfo);

	struct queue_param* p=new struct queue_param;
	p->cmdinfo=cmdinfo;
	p->dlg=pdlg;

	AfxBeginThread(&QueueThread,p,THREAD_PRIORITY_NORMAL);
/*
	CWmd5Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
//	return FALSE;
}

BOOL CWmd5App::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	m_Queue.Update();	
	return CWinApp::OnIdle(lCount);
}

void CWmd5App::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::AddToRecentFileList(lpszPathName);
}

//////////////////////////////////////////////////////////////////////
// MyCommandLineInfo Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyCommandLineInfo::MyCommandLineInfo()
{
	m_mode=CHECKMD5;
}

MyCommandLineInfo::~MyCommandLineInfo()
{

}

void MyCommandLineInfo::ParseParam(const TCHAR* pszParam,BOOL bFlag,BOOL bLast)
{
	CCommandLineInfo::ParseParam(pszParam,bFlag,bFlag);
	if(!bFlag && '@'==pszParam[0])
	{
		m_mode=GENMD5;
		m_strFileName.Format("%s",pszParam+1);
	}
}
