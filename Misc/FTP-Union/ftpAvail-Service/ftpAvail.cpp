// ftpAvail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ftpAvail.h"
#include "Winsvc.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// FTP登陆的三种状态码
#define FTP_SUCCESS 0
#define FTP_BUSY	1
#define FTP_CLOSED	2

using namespace std;

class FTPStatus
{
public:
	CString ftp;		// FTP的IP地址
	BOOL	bAvail;		
	DWORD	ErrCode;	// 错误码

	FTPStatus(CString IP="") { ftp=IP; bAvail=FALSE; ErrCode=FTP_BUSY;}
};

vector<FTPStatus> v;
UINT VerifyFTPSite(LPVOID lpParam);

void InitEventLog();
void AddEvent(DWORD id,int num,LPSTR* aszMsg);
void UpdateServer(_ConnectionPtr& pConn,int i);
void AddFtpEntry(_ConnectionPtr& pConn);
_bstr_t g_strCnn("Provider=sqloledb;Data Source=10.62.0.11;"
 		  "Initial Catalog=ftpUnion;User Id=ftpUnion;Password=qpyxzt;");

SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning=TRUE;

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
BOOL InstallMyService();
BOOL DeleteMyService();

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
	if(argc>1)
	{
		if(strcmp(argv[1],"-i")==0)
		{
			if(InstallMyService())
				printf("\n\nftpAvail Service Installed Sucessfully\n");
			else
				printf("\n\nError Installing Service\n");
		}
		else if(strcmp(argv[1],"-d")==0)
		{
			if(DeleteMyService())
				printf("\n\nftpAvail Service UnInstalled Sucessfully\n");
			else
				printf("\n\nError UnInstalling Service\n");
		}
		else
		{
			printf("\n\nUnknown Usage\n\n");
		}
	}
	else
	{
		SERVICE_TABLE_ENTRY DispatchTable[]={{"CheckFTP",ServiceMain},{NULL,NULL}};  
		StartServiceCtrlDispatcher(DispatchTable); 
	}
	return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    m_ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    m_ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    m_ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
    m_ServiceStatus.dwWin32ExitCode      = 0; 
    m_ServiceStatus.dwServiceSpecificExitCode = 0; 
    m_ServiceStatus.dwCheckPoint         = 0; 
    m_ServiceStatus.dwWaitHint           = 0; 
 
    m_ServiceStatusHandle = RegisterServiceCtrlHandler("Service1",ServiceCtrlHandler);  
    if (m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
    { 
        return; 
    }     

    m_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    m_ServiceStatus.dwCheckPoint         = 0; 
    m_ServiceStatus.dwWaitHint           = 0;  
    if (!SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus)) 
    { 

    } 

	InitEventLog();
	CoInitialize(NULL);
	_ConnectionPtr pConnection;
	pConnection.CreateInstance(__uuidof(Connection));
	
	// 在这里把要探测的FTP加入到链表中，方法如下
	// v.push_back(FTPStatus("10.100.111.146"));
	AddFtpEntry(pConnection);
	bRunning=TRUE;
	while(bRunning)
	{		
		int i;
		for (i=0; i<v.size(); i++)
			AfxBeginThread(VerifyFTPSite, (LPVOID)i);
		
		//阻塞60秒，超出60秒的均被认为是超时错误
		Sleep(60*1000);
		
		// 在这里检查本次探测情况下的各FTP状态
		// 遍历链表v，处理dwErrCode。其他地方不用改动
		for(i=0;i<v.size();i++)
			UpdateServer(pConnection,i);
		Sleep(240*1000);
	}
	CoUninitialize();
	return;
}

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
		v[i].ErrCode=FTP_SUCCESS;
	}
	catch (CInternetException* pEx)
	{
		// 失败登陆
		v[i].ErrCode=FTP_CLOSED;
		if(pEx->m_dwError == 12003)
			v[i].ErrCode = FTP_BUSY;
		pEx->Delete();
	}
	
	if (pConnect != NULL)
		pConnect->Close();
	delete pConnect;

	return 0;
}


void AddFtpEntry(_ConnectionPtr& pConn)
{
	_RecordsetPtr pRS = NULL;
	bstr_t strCmd("select IP from ServerList");
	variant_t RowsAffected;
	bstr_t strIP;
	char* s[2];
	try
	{
		pConn->Open(g_strCnn,"","",adConnectUnspecified);
		pRS = pConn->Execute(strCmd,&RowsAffected,adCmdText);
		pRS->MoveFirst();
		while(!(pRS->EndOfFile))
		{
			strIP = pRS->Fields->GetItem("IP")->Value;
			v.push_back(FTPStatus((char*)strIP));
			pRS->MoveNext();
		}
		pRS->Close();
		pConn->Close();
	}
	catch(_com_error& e)
   {
      _bstr_t bstrSource(e.Source());
      _bstr_t bstrDescription(e.Description());
	  s[0] = (LPSTR)bstrSource;
	  s[1] = (LPSTR)bstrDescription;
	  AddEvent(MSG_SS_FAIL,2,s);
   }
}

void UpdateServer(_ConnectionPtr& pConn,int i)
{
	char* s[2];
	 _RecordsetPtr pRS=NULL;
	 _variant_t RowsAffected;
	 CString str;
 	str.Format("%d",v[i].ErrCode);
	 _bstr_t strCmd("select * from ServerList where IP = \'" + v[i].ftp + "\'");
	 _bstr_t strExc("update ServerList set available = \'" + str + "\' " + "where IP = \'" + v[i].ftp + "\'");

	 try
	{
      pConn->Open(g_strCnn,"","",adConnectUnspecified);
	  pRS=pConn->Execute(strCmd,&RowsAffected,adCmdText);
	  if(!pRS->EndOfFile){
			  pConn->Execute(strExc,&RowsAffected,adCmdText);
	  }
	  else{
			  s[0] = v[i].ftp.GetBuffer(v[i].ftp.GetLength());
			  AddEvent(MSG_SERVER_NOT_FOUND,1,s);
	  }
	  pRS->Close();
	  pConn->Close();
   }

   catch(_com_error &e)
   {
      _bstr_t bstrSource(e.Source());
      _bstr_t bstrDescription(e.Description());
	  s[0] = (LPSTR)bstrSource;
	  s[1] = (LPSTR)bstrDescription;
	  AddEvent(MSG_SS_FAIL,2,s);
   }
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
    switch(Opcode) 
    { 
        case SERVICE_CONTROL_PAUSE: 
            m_ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
            break; 
 
        case SERVICE_CONTROL_CONTINUE: 
            m_ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
            break; 
 
        case SERVICE_CONTROL_STOP: 
            m_ServiceStatus.dwWin32ExitCode = 0; 
            m_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            m_ServiceStatus.dwCheckPoint    = 0; 
            m_ServiceStatus.dwWaitHint      = 0; 
 
            SetServiceStatus (m_ServiceStatusHandle,&m_ServiceStatus);

			bRunning=FALSE;

			break;
 
        case SERVICE_CONTROL_INTERROGATE: 
            break; 
    }      
    return; 
}

void InitEventLog()
{
	HKEY hk; 
    DWORD dwData; 
    char szBuf[80]; 
 
    // Add your source name as a subkey under the Application 
    // key in the EventLog registry key. 
 
    if (RegCreateKey(HKEY_LOCAL_MACHINE, 
            "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\FtpAvail", &hk)) 
	{
		return;
	}
    // Set the name of the message file. 
 
    strcpy(szBuf, "%SystemRoot%\\System\\FtpAvail.dll"); 
 
    // Add the name to the EventMessageFile subkey. 
 
    if (RegSetValueEx(hk,             // subkey handle 
            "EventMessageFile",       // value name 
            0,                        // must be zero 
            REG_EXPAND_SZ,            // value type 
            (LPBYTE) szBuf,           // pointer to value data 
            strlen(szBuf) + 1))       // length of value data 
	{
		RegCloseKey(hk); 
		return;
	}
    // Set the supported event types in the TypesSupported subkey. 
 
    dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | 
        EVENTLOG_INFORMATION_TYPE; 
 
    if (RegSetValueEx(hk,      // subkey handle 
            "TypesSupported",  // value name 
            0,                 // must be zero 
            REG_DWORD,         // value type 
            (LPBYTE) &dwData,  // pointer to value data 
            sizeof(DWORD)))    // length of value data 
	{
	    RegCloseKey(hk); 
		return;
	}

    RegCloseKey(hk); 
	
}


void AddEvent(DWORD id,int num,LPSTR* aszMsg)
{
    HANDLE h; 
    h = RegisterEventSource(NULL,  // uses local computer 
             "FtpAvail");          // source name 
    if (h == NULL) 
	{
		return;
	}
    
	if (!ReportEvent(h,           // event log handle 
            EVENTLOG_ERROR_TYPE,  // event type 
            0,                    // category zero 
            id,        // event identifier 
            NULL,                 // no user security identifier 
            num,                    // one substitution string 
            0,                    // no data 
            (LPCSTR *) aszMsg,     // pointer to string array 
            NULL))                // pointer to data 
    {
	}
	DeregisterEventSource(h); 
}

BOOL InstallMyService()
{

	char strDir[1024];
	SC_HANDLE schSCManager,schService;
	GetCurrentDirectory(1024,strDir);
	strcat(strDir,"\\ftpAvail.exe"); 

	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);  
 
	if (schSCManager == NULL) 
		return FALSE;

    LPCTSTR lpszBinaryPathName=strDir;
 
    schService = CreateService(
		schSCManager,
		"CheckFTP",
		"FtpAvail",				   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_AUTO_START,		   // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        lpszBinaryPathName,        // service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) 
        return FALSE;  
 
    CloseServiceHandle(schService); 

	return TRUE;
}


BOOL DeleteMyService()
{
	
	SC_HANDLE hService,schSCManager;

	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
 
	if (schSCManager == NULL) 
		return FALSE;	

	hService=OpenService(schSCManager,"CheckFTP",SERVICE_ALL_ACCESS);

	if (hService == NULL) 
		return FALSE;

	if(DeleteService(hService)==0)
		return FALSE;

	if(CloseServiceHandle(hService)==0)
		return FALSE;
	else
		return TRUE;
}


