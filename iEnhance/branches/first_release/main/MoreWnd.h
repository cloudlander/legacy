#ifndef _MORE_WND_H
#define _MORE_WND_H
class CIEPlus;

class CMoreDlg : 
	public CAxDialogImpl<CMoreDlg>
{
public:
	CMoreDlg(CIEPlus* p)
	{
		m_pIEplus = p;
	}
	~CMoreDlg()
	{
	}
	CIEPlus* m_pIEplus;
	HWND m_hParent;
	HWND m_hLV;
	enum { IDD = IDD_MOREDLG };
BEGIN_MSG_MAP(CMoreDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//	NOTIFY_HANDLER(IDC_PROXYLIST,NM_DBLCLK,OnDblClick)
	NOTIFY_HANDLER(IDC_PROXYLIST,LVN_GETDISPINFO,GetDispInfo)
COMMAND_ID_HANDLER(IDC_CLOSE,OnBtnClose)
COMMAND_ID_HANDLER(IDC_ACTIVATE,OnBtnActivate)
COMMAND_ID_HANDLER(IDC_DEACTIVATE,OnBtnDeActivate)
COMMAND_ID_HANDLER(IDC_REFRESH,OnBtnRefresh)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT ShowListView();
	LRESULT InitListColumn(HWND wndLV);
	LRESULT GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
//	LRESULT OnDblClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnBtnClose(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnDeActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnRefresh(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};


class CMoreWnd:public CWindowImpl<CMoreWnd,CWindow,
				CWinTraitsOR<WS_POPUPWINDOW|WS_OVERLAPPED|WS_DLGFRAME/*|WS_THICKFRAME*/|WS_VISIBLE,
					WS_EX_TOOLWINDOW,CWinTraits<> > >
{
public:
	CMoreWnd(CIEPlus* pIe):m_MoreDlg(pIe)
	{
		m_pIEplus = pIe;
	}

	CMoreDlg m_MoreDlg;
public:
	DECLARE_WND_CLASS_EX(_T("MoreWndClass"),CS_HREDRAW | CS_VREDRAW,(HBRUSH)COLOR_APPWORKSPACE+1)
	

BEGIN_MSG_MAP(CMoreWnd)
		
MESSAGE_HANDLER(WM_CREATE, OnCreate)
//MESSAGE_HANDLER(WM_PAINT, OnPaint)
MESSAGE_HANDLER(WM_CLOSE,OnClose)
MESSAGE_HANDLER(WM_SIZE,OnSize)
//NOTIFY_HANDLER(ID_LISTVIEW,LVN_GETDISPINFO,GetDispInfo)
//COMMAND_ID_HANDLER(IDC_CLOSE,OnBtnClose)
//COMMAND_ID_HANDLER(IDC_ACTIVATE,OnBtnActivate)
//COMMAND_ID_HANDLER(IDC_DEACTIVATE,OnBtnDeActivate)
//COMMAND_ID_HANDLER(IDC_REFRESH,OnBtnRefresh)
END_MSG_MAP()

private:
	CIEPlus* m_pIEplus;
	HWND m_hSetCtrl;
	HWND m_hUnsetCtrl;
	HWND m_hCloseCtrl;
	HWND m_hRefreshCtrl;
	HWND m_hListView;
/*
	BOOL InitListView();
	BOOL ShowListView();
	BOOL InitListColumn(HWND wndLV);
*/
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
/*	LRESULT GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnBtnClose(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnDeActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
	LRESULT OnBtnRefresh(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled);
*/
};

#endif