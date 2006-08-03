// OptionDlg.h : Declaration of the COptionDlg

#ifndef __OPTIONDLG_H_
#define __OPTIONDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "Utility.h"
#include "OptionData.h"
/////////////////////////////////////////////////////////////////////////////
class CSettingDlg : 
public CAxDialogImpl<CSettingDlg>
{
public:
	CSettingDlg(BOOL& bBeep,int& nComboSize):m_bBeep(bBeep),m_nComboSize(nComboSize)
	{
	}
	
	~CSettingDlg()
	{
	}
	enum { IDD = IDD_SETTINGS };
	BOOL& m_bBeep;
	int&  m_nComboSize;

	BEGIN_MSG_MAP(CSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		COMMAND_ID_HANDLER(IDOK, OnOK)
//		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()
		// Handler prototypes:
		//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DWORD bCheck=BST_UNCHECKED;
		if(m_bBeep)
			bCheck=BST_CHECKED;
		::SendMessage(GetDlgItem(IDC_BEEP),BM_SETCHECK,bCheck,0);
		SetComboSize(m_nComboSize);
		return 1;  // Let the system set the focus
	}
	
	void OnSet()
	{
		m_nComboSize=GetComboSize();
		DWORD bCheck=::SendMessage(GetDlgItem(IDC_BEEP),BM_GETCHECK,0,0);
		m_bBeep=(BST_CHECKED==bCheck ? TRUE : FALSE);
	}
	
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

	void SetComboSize(int size)
	{
		if(size<MIN_COMBO_SIZE)	size=MIN_COMBO_SIZE;
		else if(size>MAX_COMBO_SIZE) size=MAX_COMBO_SIZE;
		TCHAR str[20];
		_itot(size,str,10);
		::SetWindowText(GetDlgItem(IDC_COMBOSIZE),str);
	}

	int GetComboSize()
	{
		TCHAR str[MAX_TEXT_LENGTH];
		::GetWindowText(GetDlgItem(IDC_COMBOSIZE),str,MAX_TEXT_LENGTH);
		int size=_ttoi(str);
		if(size<MIN_COMBO_SIZE)	size=MIN_COMBO_SIZE;
		else if(size>MAX_COMBO_SIZE) size=MAX_COMBO_SIZE;
		return size;
	}
};

class CSEEntryDlg : 
	public CAxDialogImpl<CSEEntryDlg>
{
public:
	CSEEntryDlg(TCHAR* name,TCHAR* url)
	{
		if(name && url)
		{
			m_name = name;
			m_url  = url;
		}
	}
	~CSEEntryDlg()
	{
	}
	
	tstring m_name;
	tstring m_url;

	enum { IDD = IDD_SEENTRY };
BEGIN_MSG_MAP(CSEEntryDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_name.size()>0)
			::SetWindowText(GetDlgItem(IDC_NAME),m_name.c_str());
		if(m_url.size()>0)
			::SetWindowText(GetDlgItem(IDC_URL),m_url.c_str());

		RECT rc;
		GetWindowRect(&rc);
		MoveWindow(GD_X_CENTER-(rc.right-rc.left)/2,GD_Y_CENTER-(rc.bottom-rc.top)/2,
			rc.right-rc.left,rc.bottom-rc.top);

		return 1;  // Let the system set the focus
	}
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		TCHAR buf[255];
		GetDlgItemText(IDC_NAME,buf,255);
		if(buf[0] == _T('\0'))
			return 0;
		m_name = buf;
		GetDlgItemText(IDC_URL,buf,255);
		if(buf[0] == _T('\0'))
			return 0;
		m_url = buf;
		EndDialog(IDOK);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}
};

class CProxyEntryDlg : 
	public CAxDialogImpl<CProxyEntryDlg>
{
public:
	CProxyEntryDlg(ProxyEntry pe)
	{
		m_Proxy = pe;
	}
	~CProxyEntryDlg()
	{
	}
	
	ProxyEntry m_Proxy;

	enum { IDD = IDD_PROXYENTRY };
BEGIN_MSG_MAP(CProxyEntryDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		GetWindowRect(&rc);
		MoveWindow(GD_X_CENTER-(rc.right-rc.left)/2,GD_Y_CENTER-(rc.bottom-rc.top)/2,
			rc.right-rc.left,rc.bottom-rc.top);

		// 对话框的ComboBox控件竟然不是Common Control!
		int i = 0;
		TCHAR buf[10];
//		COMBOBOXEXITEM cb;
//		ZeroMemory(&cb,sizeof(cb));
//		cb.mask = CBEIF_TEXT;
		while(i<5)
		{
//			cb.iItem = i+1;
//			cb.pszText = buf;
			LoadString(_Module.m_hInst,IDS_PROXYENTRYTYPE+i,buf,10);
//			cb.cchTextMax = _tcslen(buf);
//			int x = ::SendMessage(GetDlgItem(IDC_TYPE),CBEM_INSERTITEM,0,(LPARAM)&cb);
			::SendMessage(GetDlgItem(IDC_TYPE),CB_INSERTSTRING,i,(LPARAM)buf);
			i++;
		}
//		::SendMessage(GetDlgItem(IDC_TYPE),CB_SETCURSEL,0,0);
		::SetWindowText(GetDlgItem(IDC_NAME),m_Proxy.name.c_str());
		::SetWindowText(GetDlgItem(IDC_PORT),m_Proxy.port.c_str());
//		::SetWindowText(GetDlgItem(IDC_TYPE),m_Proxy.type.c_str());
		::SendMessage(GetDlgItem(IDC_TYPE),CB_SELECTSTRING,-1,(LPARAM)m_Proxy.type.c_str());
		::SetWindowText(GetDlgItem(IDC_COMMENT),m_Proxy.comment.c_str());
		return 1;  // Let the system set the focus
	}
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		TCHAR buf[255];
		GetDlgItemText(IDC_NAME,buf,255);
		if(buf[0] == _T('\0'))
			return 0;
		m_Proxy.name = buf;
		GetDlgItemText(IDC_PORT,buf,255);
		if(buf[0] == _T('\0'))
			return 0;
		m_Proxy.port = buf;
		GetDlgItemText(IDC_TYPE,buf,255);
		m_Proxy.type = buf;
		GetDlgItemText(IDC_COMMENT,buf,255);
		m_Proxy.comment = buf;
		EndDialog(IDOK);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}
};


class CSEOptDlg : 
	public CAxDialogImpl<CSEOptDlg>
{
public:
	CSEOptDlg(CSearchEngine* p):m_TempSE(NULL)
	{
		m_pSE = p;
//		m_bChanged = FALSE;
		m_TempSE.Init(NULL);
	}
	~CSEOptDlg()
	{
	}
	
//	BOOL m_bChanged;
	CSearchEngine m_TempSE;
	HWND m_hLV;
	enum { IDD = IDD_SEDLG };
	CSearchEngine* m_pSE;
BEGIN_MSG_MAP(CSEOptDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDB_SETDEFAULT,OnSetDefault)
	COMMAND_ID_HANDLER(IDC_BTNDEL,OnDel)
	COMMAND_ID_HANDLER(IDC_BTNADD,OnAdd)
	COMMAND_ID_HANDLER(IDC_BTNDEL,OnDel)
	COMMAND_ID_HANDLER(IDC_BTNCHANGE,OnChange)
	NOTIFY_HANDLER(IDC_SELIST,NM_DBLCLK,OnDblClick)
	NOTIFY_HANDLER(IDC_SELIST,LVN_GETDISPINFO,GetDispInfo)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT ShowListView();
	LRESULT InitListColumn(HWND wndLV);
	LRESULT GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSetDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_hLV = GetDlgItem(IDC_SELIST);
		ListView_SetExtendedListViewStyle(m_hLV,LVS_EX_FULLROWSELECT);
		InitListColumn(m_hLV);
		ShowListView();
		return 1;  // Let the system set the focus
	}

};


// CProxyOptDlg
class CProxyOptDlg : 
	public CAxDialogImpl<CProxyOptDlg>
{
public:
	CProxyOptDlg(CProxyManager* p)
	{
		m_pPM = p;
		m_TempPM.Init();
	}

	~CProxyOptDlg()
	{
	}
	
	BOOL m_bSetAll;
	CProxyManager m_TempPM;
	HWND m_hLV;
	enum { IDD = IDD_PROXYDLG };
	CProxyManager* m_pPM;
BEGIN_MSG_MAP(CProxyOptDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDC_BTNADD,OnAdd)
	COMMAND_ID_HANDLER(IDC_BTNDEL,OnDel)
	COMMAND_ID_HANDLER(IDC_BTNCHANGE,OnChange)
	COMMAND_ID_HANDLER(IDC_BTNIMPORT,OnImport)
	COMMAND_ID_HANDLER(IDC_BTNEXPORT,OnExport)
	COMMAND_ID_HANDLER(IDC_SETALL,OnSetAll)
	NOTIFY_HANDLER(IDC_PROXYLIST,NM_DBLCLK,OnDblClick)
	NOTIFY_HANDLER(IDC_PROXYLIST,LVN_GETDISPINFO,GetDispInfo)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT ShowListView();
	LRESULT InitListColumn(HWND wndLV);
	LRESULT GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnImport(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnExport(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnSetAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{	
		bHandled = TRUE;
		m_bSetAll = !m_bSetAll;
		return TRUE;
	}
	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if((m_bSetAll = m_pPM->IsSetAll()))
			::SendMessage(GetDlgItem(IDC_SETALL),BM_SETCHECK,BST_CHECKED,0);
		else
			::SendMessage(GetDlgItem(IDC_SETALL),BM_SETCHECK,BST_UNCHECKED,0);

		m_hLV = GetDlgItem(IDC_PROXYLIST);
		ListView_SetExtendedListViewStyle(m_hLV,LVS_EX_FULLROWSELECT);
		InitListColumn(m_hLV);
		ShowListView();
		return 1;  // Let the system set the focus
	}

};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
/*
class CAboutDlg : 
	public CAxDialogImpl<CAboutDlg>
{
public:
	CAboutDlg()
	{
	}

	~CAboutDlg()
	{
	}
	enum { IDD = IDD_ABOUTDLG };

BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
//		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
//		EndDialog(wID);
		return 0;
	}
};
*/
/////////////////////////////////////////////////////////////////////////////
// COptionDlg
class COptionDlg : 
	public CAxDialogImpl<COptionDlg>
{
public:
	BOOL InitOptonList();
	void ChangeTab(int index);
	COptionDlg(int i,COptionData& a):m_data(a),m_ProxyOpt(a.m_pPM),
				m_SEOpt(a.m_pSE),m_Settings(a.m_bBeep,a.m_nComboSize)
	{
		m_InitView = i;
		curIndex = -1;
	}

	~COptionDlg()
	{
	}

	enum { IDD = IDD_OPTIONDLG };
	COptionData& m_data;
	int m_InitView;
	HWND m_hListView;
	CSEOptDlg m_SEOpt;
	CProxyOptDlg m_ProxyOpt;
//	CAboutDlg m_AboutOpt;
	CSettingDlg m_Settings;

	int curIndex;
BEGIN_MSG_MAP(COptionDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_OPTIONLIST,NM_CLICK,OnChangeTab)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		::EnableWindow(GetDlgItem(IDOK),FALSE);
		m_ProxyOpt.m_pPM->SetAll(m_ProxyOpt.m_bSetAll);
//		m_ProxyOpt.m_pPM->SetReqFresh();
		if(m_ProxyOpt.m_TempPM.HasChanged())
		{
			m_ProxyOpt.m_TempPM.Store();
			m_ProxyOpt.m_pPM->Init();
		}
		if(m_SEOpt.m_TempSE.HasChanged())
		{
			m_SEOpt.m_TempSE.Store();
			m_SEOpt.m_pSE->Init(NULL);
		}
		m_Settings.OnSet();
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnChangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
};

#endif //__OPTIONDLG_H_
