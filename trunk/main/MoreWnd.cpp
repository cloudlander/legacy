#include "stdafx.h"
#include "Resource.h"
#include "MoreWnd.h"
#include "ieplusatl.h"
#include "Utility.h"
#include "IEPlus.h"

LRESULT CMoreDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDlgItemText(IDC_CURPROXY,m_pIEplus->m_pPM->GetCurProxy(HTTP));
	m_hLV = GetDlgItem(IDC_PROXYLIST);
	ListView_SetExtendedListViewStyle(m_hLV,LVS_EX_FULLROWSELECT);
	InitListColumn(m_hLV);
	ShowListView();
	return 1;  // Let the system set the focus
}

LRESULT CMoreDlg::InitListColumn(HWND wndLV)
{
	TCHAR szBuf[100];
	LVCOLUMN lvc;
	int iCol;
	if(m_pIEplus)
	{
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		for (iCol = 0; iCol < 4; iCol++) 
		{	 
			lvc.iSubItem = iCol;
			lvc.pszText = szBuf;	
			lvc.cx = 100;           // width of column in pixels
	        lvc.fmt = LVCFMT_LEFT;  // left-aligned column
			LoadString(_Module.m_hInst, IDS_FIRSTCOLUMN + iCol, 
                szBuf, sizeof(szBuf));
			if (ListView_InsertColumn(wndLV, iCol, &lvc) == -1) 
				return FALSE; 
		}
		return TRUE;
	}
	else
		return FALSE;

}

LRESULT CMoreDlg::ShowListView()
{
	LVITEM lvI;
//	vector<ProxyEntry>& vec;
	if(m_pIEplus)
	{
//		vec = m_pIEplus->GetProxyList();
		// Initialize LVITEM members that are common to all items. 
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 

		// Initialize LVITEM members that are different for each item. 
		for (int index = 0; index < m_pIEplus->GetProxyList().size(); index++)
		{
   			lvI.iItem = index;
//			lvI.iImage = index;
			lvI.iSubItem = 0;
//			lvI.lParam = (LPARAM)
			lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
	
			if(ListView_InsertItem(m_hLV, &lvI) == -1)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT CMoreDlg::GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(NULL == m_pIEplus)
		return 1;
	switch(((NMLVDISPINFO*)pnmh)->item.iSubItem)
	{
	case 0:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].name.c_str();
		break;
	case 1:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].port.c_str();
		break;
	case 2:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].type.c_str();
		break;
	case 3:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].comment.c_str();
		break;

	}
	return 0;
}

LRESULT CMoreDlg::OnBtnClose(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	DestroyWindow();
	::SendMessage(m_hParent,WM_CLOSE,0,0);
	bHandled = TRUE;
	return 0;	
}

LRESULT CMoreDlg::OnBtnActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hLV,-1,LVNI_SELECTED);
	if(-1 == i)
		return 1;
	if(FALSE == m_pIEplus->m_pPM->SetProxy((m_pIEplus->GetProxyList())[i].name.c_str(),
								(m_pIEplus->GetProxyList())[i].port.c_str(),
								(m_pIEplus->GetProxyList())[i].type.c_str()))
		return 1;
	m_pIEplus->m_pPM->SetReqFresh();
	m_pIEplus->m_nSel = -1;
	SetDlgItemText(IDC_CURPROXY,m_pIEplus->m_pPM->GetCurProxy(HTTP));
	bHandled = TRUE;
	return 0;
}

LRESULT CMoreDlg::OnBtnDeActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hLV,-1,LVNI_SELECTED);
	if(-1 == i)
		return 1;
	if(FALSE == m_pIEplus->m_pPM->SetProxy(NULL,NULL,
								(m_pIEplus->GetProxyList())[i].type.c_str()))
		return 1;
	m_pIEplus->m_nSel = -1;
	m_pIEplus->m_pPM->SetReqFresh();
	SetDlgItemText(IDC_CURPROXY,m_pIEplus->m_pPM->GetCurProxy(HTTP));
	bHandled = TRUE;
	return 0;
}

LRESULT CMoreDlg::OnBtnRefresh(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	ListView_DeleteAllItems(m_hLV);
	m_pIEplus->RefreshProxyMenu();
	ShowListView();
	SetDlgItemText(IDC_CURPROXY,m_pIEplus->m_pPM->GetCurProxy(HTTP));
	return 0;
}

LRESULT CMoreWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : Add Code for message handler. Call DefWindowProc if necessary.
/*
	ATLASSERT(InitListView());
	m_hSetCtrl = CreateWindowEx(0,_T("BUTTON"),_T("Activate"),WS_CHILD | BS_PUSHBUTTON | BS_TEXT| WS_VISIBLE,
					0,0,0,0,m_hWnd,(HMENU)IDC_ACTIVATE,_Module.m_hInst,NULL);
	m_hUnsetCtrl = CreateWindowEx(0,_T("BUTTON"),_T("Deactivate"),WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
					0,0,0,0,m_hWnd,(HMENU)IDC_DEACTIVATE,_Module.m_hInst,NULL);
	m_hRefreshCtrl = CreateWindowEx(0,_T("BUTTON"),_T("Refresh"),WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
					0,0,0,0,m_hWnd,(HMENU)IDC_REFRESH,_Module.m_hInst,NULL);
	m_hCloseCtrl = CreateWindowEx(0,_T("BUTTON"),_T("Close"),WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
					0,0,0,0,m_hWnd,(HMENU)IDC_CLOSE,_Module.m_hInst,NULL);
	LOGFONT lf;
	HFONT hOld = GetFont();
//	::GetObject((HGDIOBJ)hOld,sizeof(LOGFONT),(VOID*)&lf);
	ZeroMemory((void*)&lf,sizeof(LOGFONT));
	lf.lfHeight = 14;
	lf.lfPitchAndFamily = 34;
	strcpy(lf.lfFaceName,"MS Sans Serif");
	HFONT ft = CreateFontIndirect(&lf);
	::SendMessage(m_hWnd,WM_SETFONT,(WPARAM)ft,(LPARAM)TRUE);
	::SendMessage(m_hSetCtrl,WM_SETFONT,(WPARAM)ft,(LPARAM)TRUE);
	::SendMessage(m_hUnsetCtrl,WM_SETFONT,(WPARAM)ft,(LPARAM)TRUE);
	::SendMessage(m_hRefreshCtrl,WM_SETFONT,(WPARAM)ft,(LPARAM)TRUE);
	::SendMessage(m_hCloseCtrl,WM_SETFONT,(WPARAM)ft,(LPARAM)TRUE);
*/
	m_MoreDlg.Create(m_hWnd);
	m_MoreDlg.ShowWindow(SW_SHOW);
	m_MoreDlg.m_hParent = m_hWnd;
	return 0;
}

/*
LRESULT CMoreWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
//	ReportErrMsg();
	bHandled = FALSE;
	return 0;
}
*/

LRESULT CMoreWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	bHandled = TRUE;
	return 0;
}

LRESULT CMoreWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT r;
	RECT rc; 
	m_MoreDlg.GetClientRect(&rc);
	GetWindowRect(&r);
	MoveWindow(r.left,r.top,rc.right,rc.bottom,TRUE);
	return 0;
}

/*
BOOL CMoreWnd::InitListView()
{
	HWND hWndListView;
	RECT rcl; 
// Create the list-view window in report view with label 
// editing enabled.
	ATLASSERT(m_hWnd!=NULL);
	GetClientRect (&rcl); 
	m_hListView = hWndListView = CreateWindow (WC_LISTVIEW, "", 
        WS_BORDER | WS_CHILD | LVS_REPORT | WS_VISIBLE | LVS_SINGLESEL | LVS_SHOWSELALWAYS, 
        0, 0, 0,0,//rcl.right - rcl.left, rcl.bottom - rcl.top, 
        m_hWnd, (HMENU) ID_LISTVIEW, _Module.m_hInst, NULL); 
	if (hWndListView == NULL) 
        return FALSE; 
	ListView_SetExtendedListViewStyle(m_hListView,LVS_EX_GRIDLINES|LVS_EX_FLATSB|LVS_EX_FULLROWSELECT);
	if(FALSE == InitListColumn(hWndListView))
		return FALSE;
	ShowListView();
	return TRUE;
}

BOOL CMoreWnd::InitListColumn(HWND wndLV)
{
	TCHAR szBuf[100];
	LVCOLUMN lvc;
	int iCol;
	if(m_pIEplus)
	{
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		for (iCol = 0; iCol < 4; iCol++) 
		{	 
			lvc.iSubItem = iCol;
			lvc.pszText = szBuf;	
			lvc.cx = 100;           // width of column in pixels
	        lvc.fmt = LVCFMT_LEFT;  // left-aligned column
			LoadString(_Module.m_hInst, IDS_FIRSTCOLUMN + iCol, 
                szBuf, sizeof(szBuf));
			if (ListView_InsertColumn(wndLV, iCol, &lvc) == -1) 
				return FALSE; 
		}
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMoreWnd::ShowListView()
{
	LVITEM lvI;
//	vector<ProxyEntry>& vec;
	if(m_pIEplus)
	{
//		vec = m_pIEplus->GetProxyList();
		// Initialize LVITEM members that are common to all items. 
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 

		// Initialize LVITEM members that are different for each item. 
		for (int index = 0; index < m_pIEplus->GetProxyList().size(); index++)
		{
   			lvI.iItem = index;
//			lvI.iImage = index;
			lvI.iSubItem = 0;
//			lvI.lParam = (LPARAM)
			lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
	
			if(ListView_InsertItem(m_hListView, &lvI) == -1)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT CMoreWnd::GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(NULL == m_pIEplus)
		return 1;
	switch(((NMLVDISPINFO*)pnmh)->item.iSubItem)
	{
	case 0:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].name.c_str();
		break;
	case 1:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].port.c_str();
		break;
	case 2:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].type.c_str();
		break;
	case 3:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_pIEplus->GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].comment.c_str();
		break;

	}
	return 0;
}

LRESULT CMoreWnd::OnBtnClose(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	DestroyWindow();
	bHandled = TRUE;
	return 0;	
}

LRESULT CMoreWnd::OnBtnActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if(-1 == i)
		return 1;
	if(FALSE == m_pIEplus->m_pPM->SetProxy((m_pIEplus->GetProxyList())[i].name.c_str(),
								(m_pIEplus->GetProxyList())[i].port.c_str(),
								(m_pIEplus->GetProxyList())[i].type.c_str()))
		return 1;
	m_pIEplus->m_pPM->SetReqFresh();
	bHandled = TRUE;
	return 0;
}

LRESULT CMoreWnd::OnBtnDeActivate(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if(-1 == i)
		return 1;
	if(FALSE == m_pIEplus->m_pPM->SetProxy(NULL,NULL,
								(m_pIEplus->GetProxyList())[i].type.c_str()))
		return 1;
	bHandled = TRUE;
	return 0;
}

LRESULT CMoreWnd::OnBtnRefresh(WORD hiwd,WORD lowd,HWND hwnd,BOOL& bHandled)
{
	ListView_DeleteAllItems(m_hListView);
	m_pIEplus->RefreshProxyMenu();
	ShowListView();
	return 0;
}
*/