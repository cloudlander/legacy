// QuickSaveDlg.cpp : Implementation of CQuickSaveDlg
#include "stdafx.h"
#include "QuickSaveDlg.h"
#pragma comment(lib,"wininet")

/////////////////////////////////////////////////////////////////////////////
// CQuickSaveDlg
LRESULT CQuickSaveDlg::OnShowPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;
	int i = SendMessage(::GetDlgItem(m_hWnd,IDC_SHOWPREVIEW),BM_GETSTATE,0,0);
	if(BST_UNCHECKED == (i & BST_CHECKED))
	{
		m_Flash.ShowWindow(SW_HIDE);
		m_WBrowser.ShowWindow(SW_HIDE);
		::MoveWindow(m_hListView,ListRect.left,ListRect.top,ListRect.right-ListRect.left+200,ListRect.bottom-ListRect.top,TRUE);
		m_bPreview = FALSE;
	}
	else
	{
		m_Flash.ShowWindow(SW_SHOW);
		m_WBrowser.ShowWindow(SW_SHOW);
		::MoveWindow(m_hListView,ListRect.left,ListRect.top,ListRect.right-ListRect.left,ListRect.bottom-ListRect.top,TRUE);
		m_bPreview = TRUE;
	}
	return 0;
}

LRESULT CQuickSaveDlg::OnReverse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;
	int i = ListView_GetItemCount(m_hListView);
	while(i-- > 0)
	{
		if(LVIS_SELECTED == ListView_GetItemState(m_hListView,i,LVIS_SELECTED))
		{
			ListView_SetItemState(m_hListView,i,0,LVIS_SELECTED);
		}
		else
		{
			ListView_SetItemState(m_hListView,i,LVIS_SELECTED,LVIS_SELECTED);
		}
	}
	return 0;
}

LRESULT CQuickSaveDlg::OnUnselAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;
	int i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if(-1 != i)
		::EnableWindow(::GetDlgItem(m_hWnd,IDB_SAVE),FALSE);
	while(-1 != i)
	{
		ListView_SetItemState(m_hListView,i,0,LVIS_SELECTED);
		i = ListView_GetNextItem(m_hListView,i,LVNI_SELECTED);
	}
	return 0;
}
	
LRESULT CQuickSaveDlg::OnSelAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;
	int i = ListView_GetItemCount(m_hListView)-1;
	if(-1 != i)
		::EnableWindow(::GetDlgItem(m_hWnd,IDB_SAVE),TRUE);
	for(;i>=0;i--)
	{
		ListView_SetItemState(m_hListView,i,LVIS_SELECTED,LVIS_SELECTED);
	}
	return 0;
}
	
LRESULT CQuickSaveDlg::OnSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;
	int i,j;
	i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if( -1 == i)
		return 0;
	TCHAR szPath[255];
	TCHAR szExist[255];
	TCHAR szName[255];
	if(BrowseFolder(szPath))
	{
		j = _tcslen(szPath);
		if(_T('\\') != szPath[j-1])
		{
			szPath[j] = _T('\\');
			szPath[++j] = _T('\0');
		}
		while(-1 != i)
		{
			ListView_GetItemText(m_hListView,i,2,szExist,255);
			ListView_GetItemText(m_hListView,i,0,szName,255);
			_tcscat(szPath,szName);
			if(!::CopyFile(szExist,szPath,TRUE))
				ReportErrMsg();
			i = ListView_GetNextItem(m_hListView,i,LVNI_SELECTED);
			szPath[j] = _T('\0');
		}
	}
	return 0;
}

LRESULT CQuickSaveDlg::OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int i = TabCtrl_GetCurSel(GetDlgItem(200));
	ListView_DeleteAllItems(m_hListView);
	::EnableWindow(::GetDlgItem(m_hWnd,IDB_SAVE),FALSE);
	ShowListView(i);
	Preview(0,i);
	return 0;
}

LRESULT CQuickSaveDlg::OnItemClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int i = ListView_GetNextItem(m_hListView,-1,LVNI_SELECTED);
	if(-1 != i)
		::EnableWindow(::GetDlgItem(m_hWnd,IDB_SAVE),TRUE);
	Preview(i,TabCtrl_GetCurSel(GetDlgItem(200)));
	return 0;
}


BOOL CQuickSaveDlg::InitListView()
{
	RECT rcl; 
	ATLASSERT(m_hWnd!=NULL);
	GetClientRect (&rcl); 
	m_hListView = CreateWindow (WC_LISTVIEW, "", 
        WS_BORDER | WS_CHILD | LVS_REPORT | WS_VISIBLE | LVS_SHOWSELALWAYS, 
        0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top, 
        m_hWnd, (HMENU) ID_LISTVIEW, _Module.m_hInst, NULL); 
	if (m_hListView == NULL) 
        return FALSE; 
	ListView_SetExtendedListViewStyle(m_hListView,LVS_EX_GRIDLINES|LVS_EX_FLATSB|LVS_EX_FULLROWSELECT);
	if(FALSE == InitListColumn())
		return FALSE;
	ShowListView(0);
	return TRUE;
}

BOOL CQuickSaveDlg::InitListColumn()
{
	TCHAR szBuf[100];
	LVCOLUMN lvc;
	int iCol;
	if(m_pFrameWB)
	{
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		for (iCol = 0; iCol < 3; iCol++) 
		{	 
			lvc.iSubItem = iCol;
			lvc.pszText = szBuf;
			lvc.cx = 300;           // width of column in pixels
			if(0 == iCol)
				lvc.cx = 100;
	        lvc.fmt = LVCFMT_LEFT;  // left-aligned column
			LoadString(_Module.m_hInst, IDS_FIRSTCOLUMNQS + iCol, 
                szBuf, sizeof(szBuf));
			if (ListView_InsertColumn(m_hListView, iCol, &lvc) == -1) 
				return FALSE; 
		}
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CQuickSaveDlg::ShowListView(int type)
{
	if(!m_pFrameWB)
		return FALSE;
	if(0 == type)
	{
		m_Flash.ShowWindow(SW_SHOW);
		m_WBrowser.ShowWindow(SW_HIDE);
	}
	else
	{
		m_Flash.ShowWindow(SW_HIDE);
		m_WBrowser.ShowWindow(SW_SHOW);
	}

    CComPtr<IDispatch> pDisp;
    if (SUCCEEDED(m_pFrameWB->get_Document(&pDisp)) && pDisp)
    {
		CComPtr<IHTMLDocument2> pDoc;
        HRESULT hr = pDisp->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc);
		pDisp.Release();
        if (FAILED(hr))
            return FALSE;
		return ShowFrame(type,pDoc);
	}

	else
		return FALSE;
}

BOOL CQuickSaveDlg::ShowFrame(int type,CComPtr<IHTMLDocument2>& pFrameDoc)
{
		HRESULT hr;
		long cFrames;
		CComPtr<IHTMLWindow2> pWnd2;
		CComPtr<IHTMLWindow2> pFrameWindow2;
		CComPtr<IHTMLFramesCollection2> pFrameColl;
		CComPtr<IHTMLDocument2> pDoc;
		ShowDoc(type,pFrameDoc);
		hr = pFrameDoc->get_parentWindow(&pWnd2);
		if(FAILED(hr) || !pWnd2)
			return FALSE;
		hr = pWnd2->get_frames(&pFrameColl);
		if (SUCCEEDED(hr = pFrameColl->get_length( &cFrames )))
		{
			for ( int i=0; i < cFrames ; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				VARIANT frame;
			
				if (SUCCEEDED(hr = pFrameColl->item( &vIndex, &frame ))){
					hr = frame.pdispVal->QueryInterface(IID_IHTMLWindow2,  (void**)&pFrameWindow2);
					hr = pFrameWindow2->get_document(&pDoc);
					if(SUCCEEDED(hr) && pDoc!=NULL)
					{
						ShowDoc(type,pDoc);
						ShowFrame(type,pDoc);
					}
					pFrameWindow2.Release();
					pDoc.Release();
				}
			}

		}
		return FALSE;
}

BOOL CQuickSaveDlg::ShowDoc(int type,CComPtr<IHTMLDocument2>& pDoc)
{
	HRESULT hr;
	TCHAR szType[50];
	if(0 == type)
		_tcscpy(szType,_T("object"));
	else if(1 == type)
		_tcscpy(szType,_T("img"));
	else	
		return FALSE;
  
	CComPtr<IHTMLElementCollection> pElemColl;
	CComPtr<IDispatch> pDisp,pDisp2;

    BSTR bstr;
	BSTR baseSrc;
	BSTR domain;
	BSTR src;
	BSTR clsid;
	BSTR PlugPage;
	_bstr_t strClsid;
	_bstr_t strPlugpage;
	_bstr_t srct;
	char sss[250];

	CComPtr<IHTMLDocument2> pDoc2;
	CComPtr<IHTMLObjectElement> pObj;

	CComPtr<IHTMLEmbedElement> pEmb;
	CComPtr<IShockwaveFlash> pFlash;
	CComPtr<IHTMLImgElement> pImg;
    
	hr = pDoc->get_all(&pElemColl);
//  pDoc->Release();

    if (FAILED(hr))
        return FALSE;

    long lNumElems = 0;
    pElemColl->get_length(&lNumElems);

    for (int i = 0; i < lNumElems; i++)
    {
           _variant_t vtItem((long)i), vtEmpty;
           hr = pElemColl->item(vtItem, vtEmpty, &pDisp);

            if (FAILED(hr))
               break;

            // Get the IHTMLElement interface
            CComPtr<IHTMLElement> pElem;

            hr = pDisp->QueryInterface(IID_IHTMLElement, (LPVOID*)&pElem);
            pDisp.Release();

            if (FAILED(hr))
               break;

            hr = pElem->get_tagName(&bstr);
			if (SUCCEEDED(hr))
            {
               _bstr_t bstrTagName(bstr);
				if (!lstrcmpi(bstrTagName, szType))
				{
					if(0 == type)
					{
						pElem->QueryInterface(IID_IHTMLObjectElement,(LPVOID*)&pObj);
						pObj->get_classid(&clsid);
						strClsid = clsid;
						if(!lstrcmpi(strClsid,"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"))
						{
							hr = pObj->get_object(&pDisp);
							ATLASSERT(SUCCEEDED(hr));
							pDisp->QueryInterface(&pFlash);
							pFlash->get_Movie(&src);
					        srct = src;
							USES_CONVERSION;
							char *s = W2A(srct);
							if(s[0] == '/')
							{
								m_pFrameWB->get_Document(&pDisp2);
								HRESULT hr = pDisp2->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc2);
								pDoc2->get_domain(&domain);
								pDoc2.Release();
								pDisp2.Release();
								char *d = W2A(domain);
								strcpy(sss,"http://");
								strcat(sss,d);
								strcat(sss,s);
								srct = sss;
							}
							else if(0 != strncmp(s,"http://",7))
							{
								pObj->get_BaseHref(&baseSrc);
								srct = _bstr_t(baseSrc)+_bstr_t(src);
							}
							InsertFlash(i,srct);
							pFlash.Release();
						}
						pDisp.Release();
						pObj.Release();
					}
					else if(1 == type)
					{
						pElem->QueryInterface(IID_IHTMLImgElement,(LPVOID*)&pImg);
						InsertImg(i,pImg);
						pImg.Release();	
					}
					else
						return FALSE;
				}
				if(0 == type && !lstrcmpi(bstrTagName,_T("EMBED")))
				{
					pElem->QueryInterface(IID_IHTMLEmbedElement,(LPVOID*)&pEmb);
					pEmb->get_pluginspage(&PlugPage);
					strPlugpage = PlugPage;
					if(!lstrcmpi(strPlugpage,"http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"))
					{
						hr = pEmb->get_src(&src);
						InsertFlash(i,src);
					}
					pEmb.Release();
				}
			}
			else
				return FALSE;
	}
	return TRUE;
}

BOOL CQuickSaveDlg::InsertImg(int i,CComPtr<IHTMLImgElement>& pImg)
{
 	LVITEM lvI;
	lvI.mask = LVIF_TEXT ; 
	lvI.state = 0; 
	lvI.stateMask = 0;

	LVFINDINFO lvfd;
	lvfd.flags = LVFI_STRING;

	LPINTERNET_CACHE_ENTRY_INFO picei;
	DWORD length=sizeof(INTERNET_CACHE_ENTRY_INFO);
	BSTR filename;
	BSTR url;

	_bstr_t szStr;
   	
	lvI.iItem = i;
//	lvI.iImage = index;
	lvI.iSubItem = 0;
//	lvI.lParam = (LPARAM)
	pImg->get_href((BSTR*)&url);
	szStr = url;
	picei = (LPINTERNET_CACHE_ENTRY_INFO)malloc(length);
//					if(GetUrlCacheEntryInfo((TCHAR*)szStr,picei,&length))
//					{
	GetUrlCacheEntryInfo((TCHAR*)szStr,picei,&length);
	if(length > sizeof(INTERNET_CACHE_ENTRY_INFO))
	{
		free(picei);
		picei = (LPINTERNET_CACHE_ENTRY_INFO)malloc(length);
		GetUrlCacheEntryInfo((TCHAR*)szStr,picei,&length);
	}
	pImg->get_nameProp(&filename);
	szStr = filename;
	lvI.pszText = (TCHAR*) szStr;	
	lvfd.psz = lvI.pszText;
	if(-1 == ListView_FindItem(m_hListView,-1,&lvfd))
	{
		if(-1 != (lvI.iItem  = ListView_InsertItem(m_hListView, &lvI)))
		{
			lvI.iSubItem = 1;
			szStr = url;
			lvI.pszText = (TCHAR*)szStr;
			ListView_SetItem(m_hListView,&lvI);
			lvI.iSubItem = 2;
			lvI.pszText = picei->lpszLocalFileName;
			ListView_SetItem(m_hListView,&lvI);
		}
		else
			return FALSE;
		free(picei);
	}
	return TRUE;
}

BOOL CQuickSaveDlg::InsertFlash(int i,BSTR src)
{
 	LVITEM lvI;
	lvI.mask = LVIF_TEXT ; 
	lvI.state = 0; 
	lvI.stateMask = 0;

	LVFINDINFO lvfd;
	lvfd.flags = LVFI_STRING;

	LPINTERNET_CACHE_ENTRY_INFO picei;
	DWORD length=sizeof(INTERNET_CACHE_ENTRY_INFO);
	char filename[250];
	_bstr_t urlt(src);
   	USES_CONVERSION;
	char szUrl[250];
	int index;

	strcpy(szUrl,W2A(src));
	lvI.iItem = i;
//	lvI.iImage = index;
	lvI.iSubItem = 0;
//	lvI.lParam = (LPARAM)
	picei = (LPINTERNET_CACHE_ENTRY_INFO)malloc(length);
	GetUrlCacheEntryInfo(szUrl,picei,&length);
//	ReportErrMsg();
	if(length > sizeof(INTERNET_CACHE_ENTRY_INFO))
	{
		free(picei);
		picei = (LPINTERNET_CACHE_ENTRY_INFO)malloc(length);
		GetUrlCacheEntryInfo(szUrl,picei,&length);
	}

	index = strlen(szUrl);
	for(;index>=0;index--)
	{
		if(szUrl[index]=='/')
			break;
	}
	strcpy(filename,szUrl+index+1);
	
	lvI.pszText = filename;	
	lvfd.psz = lvI.pszText;

//	if(-1 == ListView_FindItem(m_hListView,-1,&lvfd))
//	{
		if(-1 != (lvI.iItem  = ListView_InsertItem(m_hListView, &lvI)))
		{
/*			lvI.iSubItem = 1;
			szStr = url;
			lvI.pszText = (TCHAR*)szStr;
			ListView_SetItem(m_hListView,&lvI);
*/			lvI.iSubItem = 1;
			lvI.pszText = szUrl;
			ListView_SetItem(m_hListView,&lvI);
			lvI.iSubItem = 2;
			lvI.pszText = picei->lpszLocalFileName;
			ListView_SetItem(m_hListView,&lvI);
		}
		else
			return FALSE;
		free(picei);
//	}
	return TRUE;
}

void CQuickSaveDlg::Preview(int index, int type)
{
	if(!m_bPreview)
		return;
	TCHAR buf[255];
	ListView_GetItemText(m_hListView,index,2,buf,255);
	if(0 == type)
	{
		IShockwaveFlashPtr pfsh;
		m_Flash.QueryControl(__uuidof(IShockwaveFlash),(void**)&pfsh);
		pfsh->PutMovie(buf);
	}
	else
	{
		variant_t vtEmpty;
		IWebBrowser2Ptr pWeb;
		m_WBrowser.QueryControl(__uuidof(IWebBrowser2),(void**)&pWeb);
		pWeb->Navigate(bstr_t(buf),&vtEmpty,&vtEmpty,&vtEmpty,&vtEmpty);
	}
}

BOOL CQuickSaveDlg::BrowseFolder(TCHAR *szPath)
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = ("Select a folder");
	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

    if ( pidl != 0 )
    {
        // get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, szPath );

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
		return TRUE;
    }
	return FALSE;
}
