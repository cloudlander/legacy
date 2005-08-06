// QuickSaveDlg.h : Declaration of the CQuickSaveDlg

#ifndef __QUICKSAVEDLG_H_
#define __QUICKSAVEDLG_H_
#include <windowsx.h>
#include <shlobj.h>
#include "resource.h"       // main symbols
#include "Utility.h"
/////////////////////////////////////////////////////////////////////////////
// CQuickSaveDlg
class CQuickSaveDlg : 
	public CAxDialogImpl<CQuickSaveDlg>
{
public:
	CQuickSaveDlg(const CComPtr<IWebBrowser2>& pFW)
	{
		m_bPreview = TRUE;
		m_hListView = NULL;
		if(pFW)
			m_pFrameWB = pFW;
	}

	~CQuickSaveDlg()
	{
	}

	enum { IDD = IDD_QUICKSAVEDLG };
private:
	BOOL BrowseFolder(TCHAR* szPath);
	void Preview(int index,int type);
	BOOL InsertImg(int i,CComPtr<IHTMLImgElement>& pImg);
	BOOL InsertFlash(int i,BSTR src);
	BOOL ShowDoc(int type,CComPtr<IHTMLDocument2>& pDoc);
	BOOL ShowFrame(int type,CComPtr<IHTMLDocument2>& pFrameDoc);
	BOOL ShowListView(int type);
	BOOL InitListColumn();
	BOOL InitListView();
	RECT ListRect;
	HWND m_hListView;
	HWND m_hWndTab;
	BOOL m_bPreview;
	CAxWindow m_Flash;
	CAxWindow m_WBrowser;
	CComPtr<IWebBrowser2> m_pFrameWB;
public:
BEGIN_MSG_MAP(CQuickSaveDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	NOTIFY_HANDLER(200,TCN_SELCHANGE,OnSelChange)
	NOTIFY_HANDLER(ID_LISTVIEW,NM_CLICK,OnItemClick)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDB_UNSELALL, OnUnselAll)
	COMMAND_ID_HANDLER(IDB_SAVE,OnSave)
	COMMAND_ID_HANDLER(IDB_SELALL,OnSelAll)
	COMMAND_ID_HANDLER(IDB_UNSELALL,OnUnselAll)
	COMMAND_ID_HANDLER(IDB_REVERSE,OnReverse)
	COMMAND_ID_HANDLER(IDC_SHOWPREVIEW,OnShowPreview)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rcClient;
		GetClientRect(&rcClient);
		m_hWndTab = CreateWindow( 
        WC_TABCONTROL, "", 
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER, 
        0, 0, rcClient.right, rcClient.bottom, 
        m_hWnd, (HMENU)200, _Module.m_hInst, NULL 
        ); 
		
		::EnableWindow(::GetDlgItem(m_hWnd,IDB_SAVE),FALSE);

		LOGFONT lf;
		HFONT hOld = GetFont();
		::GetObject((HGDIOBJ)hOld,sizeof(LOGFONT),(VOID*)&lf);
		strcpy(lf.lfFaceName,"MS Sans Serif");
		lf.lfHeight++;
		::SendMessage(m_hWndTab,WM_SETFONT,(WPARAM)CreateFontIndirect(&lf),(LPARAM)TRUE);
		TCITEM  tie;
		tie.mask = TCIF_TEXT | TCIF_IMAGE; 
		tie.iImage = -1; 
		tie.pszText = "Flash"; 
        if (TabCtrl_InsertItem(m_hWndTab, 0, &tie) == -1) { 
            ::DestroyWindow(m_hWndTab); 
            return NULL; 
        } 
		tie.pszText = "images"; 
        if (TabCtrl_InsertItem(m_hWndTab, 1, &tie) == -1) { 
            ::DestroyWindow(m_hWndTab); 
            return NULL; 
        } 
		m_Flash.Attach(GetDlgItem(IDC_SHOCKWAVEFLASH));
		m_WBrowser.Attach(GetDlgItem(IDC_WBROWSER));
		InitListView();
		SendMessage(WM_SIZE,0,MAKELONG(rcClient.right,rcClient.bottom));
		::GetClientRect(m_hListView,&ListRect);
		::MapWindowPoints(m_hListView,m_hWnd,(LPPOINT)&ListRect,2);
		ListRect.bottom+=18;

		::SendMessage(GetDlgItem(IDC_SHOWPREVIEW),BM_SETCHECK,BST_CHECKED,0);
		::GetWindowRect(m_hWnd,&rcClient);
		MoveWindow(GD_X_CENTER-(rcClient.right-rcClient.left)/2,GD_Y_CENTER-(rcClient.bottom-rcClient.top)/2,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top);
		return 1;  // Let the system set the focus
	}

	LRESULT OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnItemClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnUnselAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnSelAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnReverse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnShowPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		HDWP hdwp; 
		RECT rc;
		SetRect(&rc, 0, 0, 
                GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)-50); 
        TabCtrl_AdjustRect(m_hWndTab, FALSE, &rc); 
 
        // Size the tab control to fit the client area. 
        hdwp = BeginDeferWindowPos(3); 
        ::DeferWindowPos(hdwp, m_hWndTab, NULL, 0, 0, 
                  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)-50, 
                  SWP_NOMOVE | SWP_NOZORDER 
                  ); 
 
        ::DeferWindowPos(hdwp, 
                    m_hListView, HWND_TOP, rc.left, rc.top, 
                    rc.right - rc.left-200, rc.bottom - rc.top, 0 
                    ); 
		::DeferWindowPos(hdwp,m_Flash.m_hWnd,HWND_TOP,rc.right-200,rc.top,
					200,rc.bottom-rc.top,0);
		::DeferWindowPos(hdwp,m_WBrowser.m_hWnd,HWND_TOP,rc.right-200,rc.top,
					200,rc.bottom-rc.top,0);
        EndDeferWindowPos(hdwp); 
		return 0;
	}
};

#endif //__QUICKSAVEDLG_H_
