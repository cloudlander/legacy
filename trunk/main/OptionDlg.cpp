// OptionDlg.cpp : Implementation of COptionDlg
#include "stdafx.h"
#include "OptionDlg.h"

LRESULT CSEOptDlg::InitListColumn(HWND wndLV)
{
	TCHAR szBuf[100];
	LVCOLUMN lvc;
	int iCol;
	if(m_pSE)
	{
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		for (iCol = 0; iCol < 2; iCol++) 
		{	 
			lvc.iSubItem = iCol;
			lvc.pszText = szBuf;	
			lvc.cx = iCol == 0 ? 120 : 250 ;           // width of column in pixels
	        lvc.fmt = LVCFMT_LEFT;  // left-aligned column
			LoadString(_Module.m_hInst, IDS_SEFIRSTCOLUMN + iCol, 
                szBuf, sizeof(szBuf));
			if (ListView_InsertColumn(wndLV, iCol, &lvc) == -1) 
				return FALSE; 
		}
		return TRUE;
	}
	else
		return FALSE;
}

LRESULT CSEOptDlg::ShowListView()
{
	LVITEM lvI;
	if(m_pSE)
	{
		// Initialize LVITEM members that are common to all items. 
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 

		// Initialize LVITEM members that are different for each item. 
		for (int index = 0; index < m_TempSE.GetSize(); index++)
		{
   			lvI.iItem = index;
//			lvI.iImage = index;
			lvI.iSubItem = 0;
			lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
	
			if(ListView_InsertItem(m_hLV,&lvI) == -1)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT CSEOptDlg::OnSetDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hLV,-1,LVNI_SELECTED);
	if(-1 != i)
	{
		m_TempSE.SetDefault(i);
	}
	return 0;
}

LRESULT CSEOptDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CSEEntryDlg dlg(NULL,NULL);
	if(IDOK == dlg.DoModal())
	{
		m_TempSE.Add(dlg.m_name.c_str(),dlg.m_url.c_str());
		int i = ListView_GetItemCount(m_hLV);
		LVITEM lvI;
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 
		lvI.iItem = i;
		lvI.iSubItem = 0;
		lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
		if(ListView_InsertItem(m_hLV,&lvI) == -1)
			return 1;
	}
	return 0;
}

LRESULT CSEOptDlg::OnDblClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	BOOL h;
	OnChange(0,0,NULL,h);
	return h;
}

LRESULT CSEOptDlg::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR name[255];
	TCHAR url[255];
	int i = ListView_GetNextItem(m_hLV,-1,LVNI_SELECTED);
	if(-1 != i)
	{
		ListView_GetItemText(m_hLV,i,0,name,255);
		ListView_GetItemText(m_hLV,i,1,url,255);
		CSEEntryDlg dlg(name,url);
		if(IDOK == dlg.DoModal())
		{
			m_TempSE.SetChanged();
			(m_TempSE.GetSE())[i].name = dlg.m_name;
			(m_TempSE.GetSE())[i].url = dlg.m_url;
			ListView_RedrawItems(m_hLV,i,i);
		}
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CSEOptDlg::OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int end = ListView_GetItemCount(m_hLV);
	int i = ListView_GetNextItem(m_hLV,end-1,LVNI_ABOVE|LVNI_SELECTED);
	CWindow wnd;
	if(-1 != i)
	{	
		wnd.Attach(m_hLV);
		wnd.SetRedraw(FALSE);
		while(-1 != i)
		{
			ListView_DeleteItem(m_hLV,i);
			m_TempSE.Delete(i);
			i = ListView_GetNextItem(m_hLV,i,LVNI_ABOVE|LVNI_SELECTED);
		}
		wnd.SetRedraw();
	}
	end = ListView_GetItemCount(m_hLV);
	if(LVIS_SELECTED == ListView_GetItemState(m_hLV,end-1,LVIS_SELECTED))
	{
		ListView_DeleteItem(m_hLV,end-1);
		m_TempSE.Delete(end-1);
	}
	return 0;
}

LRESULT CSEOptDlg::GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(NULL == m_pSE)
		return 1;
	switch(((NMLVDISPINFO*)pnmh)->item.iSubItem)
	{
	case 0:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempSE.GetName(((NMLVDISPINFO*)pnmh)->item.iItem));
		break;
	case 1:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempSE.GetUrl(((NMLVDISPINFO*)pnmh)->item.iItem));
		break;
	}
	return 0;
}


LRESULT CProxyOptDlg::InitListColumn(HWND wndLV)
{
	TCHAR szBuf[100];
	LVCOLUMN lvc;
	int iCol;
	if(m_pPM)
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

LRESULT CProxyOptDlg::ShowListView()
{
	LVITEM lvI;
	if(m_pPM)
	{
		// Initialize LVITEM members that are common to all items. 
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 

		// Initialize LVITEM members that are different for each item. 
		for (int index = 0; index < m_TempPM.GetProxyList().size(); index++)
		{
   			lvI.iItem = index;
//			lvI.iImage = index;
			lvI.iSubItem = 0;
			lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
	
			if(ListView_InsertItem(m_hLV,&lvI) == -1)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT CProxyOptDlg::GetDispInfo(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(NULL == m_pPM)
		return 1;
	switch(((NMLVDISPINFO*)pnmh)->item.iSubItem)
	{
	case 0:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempPM.GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].name.c_str();
		break;
	case 1:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempPM.GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].port.c_str();
		break;
	case 2:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempPM.GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].type.c_str();
		break;
	case 3:
		((NMLVDISPINFO*)pnmh)->item.pszText = (char*)
			(m_TempPM.GetProxyList())[((NMLVDISPINFO*)pnmh)->item.iItem].comment.c_str();
		break;

	}
	return 0;
}

LRESULT CProxyOptDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ProxyEntry pe;
	CProxyEntryDlg dlg(pe);
	if(IDOK == dlg.DoModal())
	{
		m_TempPM.AddProxy(dlg.m_Proxy);
		int i = ListView_GetItemCount(m_hLV);
		LVITEM lvI;
		lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
		lvI.state = 0; 
		lvI.stateMask = 0; 
		lvI.iItem = i;
		lvI.iSubItem = 0;
		lvI.pszText = LPSTR_TEXTCALLBACK; // sends an LVN_GETDISPINFO message.									  
		if(ListView_InsertItem(m_hLV,&lvI) == -1)
			return 1;
	}
	return 0;
}

LRESULT CProxyOptDlg::OnDblClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	BOOL h;
	OnChange(0,0,NULL,h);
	return h;
}

LRESULT CProxyOptDlg::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR name[255];
	TCHAR port[255];
	TCHAR type[255];
	TCHAR comment[255];
	int i = ListView_GetNextItem(m_hLV,-1,LVNI_SELECTED);
	if(-1 != i)
	{
		ListView_GetItemText(m_hLV,i,0,name,255);
		ListView_GetItemText(m_hLV,i,1,port,255);
		ListView_GetItemText(m_hLV,i,2,type,255);
		ListView_GetItemText(m_hLV,i,3,comment,255);
		CProxyEntryDlg dlg(ProxyEntry(name,port,type,comment));
		if(IDOK == dlg.DoModal())
		{
			m_TempPM.SetChanged();
			(m_TempPM.GetProxyList())[i] = dlg.m_Proxy;
			ListView_RedrawItems(m_hLV,i,i);
		}
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CProxyOptDlg::OnDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int end = ListView_GetItemCount(m_hLV);
	int i = ListView_GetNextItem(m_hLV,end-1,LVNI_ABOVE|LVNI_SELECTED);
	CWindow wnd;
	if(-1 != i)
	{	
		wnd.Attach(m_hLV);
		wnd.SetRedraw(FALSE);
		while(-1 != i)
		{
			ListView_DeleteItem(m_hLV,i);
			m_TempPM.Delete(i);
			i = ListView_GetNextItem(m_hLV,i,LVNI_ABOVE|LVNI_SELECTED);
		}
		wnd.SetRedraw();
	}
	end = ListView_GetItemCount(m_hLV);
	if(LVIS_SELECTED == ListView_GetItemState(m_hLV,end-1,LVIS_SELECTED))
	{
		ListView_DeleteItem(m_hLV,end-1);
		m_TempPM.Delete(end-1);
	}
	return 0;
}

LRESULT CProxyOptDlg::OnImport(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char szFile[260];       // buffer for file name
	szFile[0] = '\0';
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		m_TempPM.ImportProxyList((TCHAR*)ofn.lpstrFile);
		ListView_DeleteAllItems(m_hLV);
		ShowListView();
		return 0;
	}
	return 1;
}

LRESULT CProxyOptDlg::OnExport(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char szFile[260];       // buffer for file name
	szFile[0] = '\0';
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0");
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT;
	// Display the Open dialog box. 
	if (GetSaveFileName(&ofn)==TRUE) 
	{
		tstring buf=tstring(ofn.lpstrFile);//+tstring(ofn.lpstrFilter);
		m_TempPM.ExportProxyList(buf.c_str());
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// COptionDlg
LRESULT COptionDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		RECT rc;
		GetWindowRect(&rc);
		MoveWindow(GD_X_CENTER-(rc.right-rc.left)/2,GD_Y_CENTER-(rc.bottom-rc.top)/2,
			rc.right-rc.left,rc.bottom-rc.top);

	m_hListView = GetDlgItem(IDC_OPTIONLIST);
	InitOptonList();
	RECT rect;
	::GetClientRect(GetDlgItem(IDC_GBOX),&rect);
	::MapWindowPoints(GetDlgItem(IDC_GBOX),m_hWnd,(LPPOINT)&rect,2);
	m_SEOpt.Create(m_hWnd);
	m_SEOpt.MoveWindow(&rect);
	m_ProxyOpt.Create(m_hWnd);
	m_ProxyOpt.MoveWindow(&rect);
	m_Settings.Create(m_hWnd);
	m_Settings.MoveWindow(&rect);
//	m_AboutOpt.Create(m_hWnd);
//	m_AboutOpt.MoveWindow(&rect);
	ChangeTab(m_InitView);
	return 1;  // Let the system set the focus
}

void COptionDlg::ChangeTab(int index)
{
	if(curIndex == -1)
		curIndex = index;
	else
	{
		if(curIndex == index)
			return;
		switch(curIndex)
		{
		case 0:
				m_SEOpt.ShowWindow(SW_HIDE);
				break;
		case 1:
				m_ProxyOpt.ShowWindow(SW_HIDE);
				break;
		case 2:
//				m_AboutOpt.ShowWindow(SW_HIDE);
				m_Settings.ShowWindow(SW_HIDE);
				break;
		}
		curIndex = index;
	}
	switch(index)
	{
	case 0:
			m_SEOpt.ShowWindow(SW_SHOW);
			break;
	case 1:
			m_ProxyOpt.ShowWindow(SW_SHOW);
			break;
	case 2:
//			m_AboutOpt.ShowWindow(SW_SHOW);
			m_Settings.ShowWindow(SW_SHOW);
			break;
	}
}

BOOL COptionDlg::InitOptonList()
{
 	LVITEM lvI;
	lvI.mask = LVIF_TEXT ; 
	lvI.state = 0; 
	lvI.stateMask = 0;
//	lvI.iImage = index;
	lvI.iSubItem = 0;
	lvI.iItem = 0;
	lvI.pszText = _T("搜索引擎");
	if(ListView_InsertItem(m_hListView, &lvI) == -1)
		return FALSE;
	lvI.iItem = 1;
	lvI.pszText = _T("代理服务器");
	if(ListView_InsertItem(m_hListView, &lvI) == -1)
		return FALSE;
	lvI.iItem = 2;
//	lvI.pszText = _T("关于");
	lvI.pszText = _T("高级设置");
	if(ListView_InsertItem(m_hListView, &lvI) == -1)
		return FALSE;
	return TRUE;
}

LRESULT COptionDlg::OnChangeTab(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if(-1 == i)
		return 1;
	ChangeTab(i);
	bHandled = TRUE;
	return 0;
}