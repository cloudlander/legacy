// IEPlus.cpp : Implementation of CIEPlus
#include "stdafx.h"
#define WM_IENHEXT_CLOSESESSION		WM_USER+111
#define BTNS_WHOLEDROPDOWN  0x0080
#define TBSTYLE_EX_MIXEDBUTTONS             0x00000008
#define BTNS_SHOWTEXT   0x0040
#include <commctrl.h>
#include <exdispid.h>
#include <ras.h>
#include "IEPlusATL.h"
#include "IEPlus.h"
#include "Utility.h"
#include "OptionData.h"
#include "Utility.h"
#pragma comment(lib,"comctl32")
#pragma comment(lib,"comdlg32")
#pragma  data_seg("shr")
BOOL g_bCloseCurIE=FALSE;
#pragma  data_seg()
#pragma  comment(linker,"/section:shr,rws")
/////////////////////////////////////////////////////////////////////////////
// CIEPlus
CIEPlus::CIEPlus()
{
	m_nSel = -1;
	m_nClosed=0;
	m_pSite = NULL;

	m_hWnd = NULL;
	m_hWebBrower = NULL;
	m_hwndParent = NULL;
	m_hwndToolbar = NULL;
	m_hedit=NULL;
	m_hCombo=NULL;
	m_pMoreWnd = NULL;
	
	m_pFrameWB = NULL;
	m_dwWBCookie=0;
	m_pDocUI = NULL;
	m_dwViewMode = 0;
	m_dwBandID = 0;
	m_bFocus = FALSE;
	
	m_SEMenu = NULL;
	m_PopupMenu = NULL;
	m_HttpMenu = NULL;
	m_MainMenu = NULL;
	
	m_pSE = NULL;
	m_pPM = NULL;
	m_pPlugMgr=NULL;


	orgProc=0;
	orgEditProc=0;
}
/**************************************************************************

   CIEPlus::~CIEPlus()

**************************************************************************/

CIEPlus::~CIEPlus()
{
//	g_pIEPlus=NULL;
//	SetWindowLong(m_hedit,GWL_WNDPROC,(LONG)orgEditProc);
//this should have been freed in a call to SetSite(NULL), but just to be safe
	if(m_pSite)
	{
		m_pSite->Release();
		m_pSite = NULL;
	}
/*	if(m_pFrameWB)
	{
		m_pFrameWB->Release();
		m_pFrameWB = NULL;
	}
*/
	if(m_pDocUI)
	{
		m_pDocUI->Release();
		m_pDocUI = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////
//
// IOleWindow Implementation
//

/**************************************************************************

   CIEPlus::GetWindow()
   
**************************************************************************/

STDMETHODIMP CIEPlus::GetWindow(HWND *phWnd)
{
	*phWnd = m_hWnd;

	return S_OK;
}

/**************************************************************************

   CIEPlus::ContextSensitiveHelp()
   
**************************************************************************/

STDMETHODIMP CIEPlus::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
// IDockingWindow Implementation
//

/**************************************************************************

   CIEPlus::ShowDW()
   
**************************************************************************/

STDMETHODIMP CIEPlus::ShowDW(BOOL fShow)
{
	if(m_hWnd)
	{
		if(fShow)
		{
		//show our window
			ShowWindow(m_hWnd, SW_SHOW);
		}
		else
		{
		//hide our window
			ShowWindow(m_hWnd, SW_HIDE);
		}
	}

	return S_OK;
}

/**************************************************************************

   CIEPlus::CloseDW()
   
**************************************************************************/

STDMETHODIMP CIEPlus::CloseDW(DWORD dwReserved)
{
	ShowDW(FALSE);

	if(IsWindow(m_hWnd))
		DestroyWindow(m_hWnd);

	m_hWnd = NULL;
   
	return S_OK;
}

/**************************************************************************

   CIEPlus::ResizeBorderDW()
   
**************************************************************************/

STDMETHODIMP CIEPlus::ResizeBorderDW(   LPCRECT prcBorder, 
                                          IUnknown* punkSite, 
                                          BOOL fReserved)
{
	/*
	This method is never called for Band Objects.
	*/
	return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
// IInputObject Implementation
//

/**************************************************************************

   CIEPlus::UIActivateIO()
   
**************************************************************************/

STDMETHODIMP CIEPlus::UIActivateIO(BOOL fActivate, LPMSG pMsg)
{
//	MessageBox(m_hWnd,"UIActivateIO","info",MB_OK|MB_ICONINFORMATION);
	if(fActivate&&m_hedit!=NULL)
	{
		SetFocus(m_hedit);
	}
	return S_OK;
}

/**************************************************************************

   CIEPlus::HasFocusIO()
   
   If this window or one of its decendants has the focus, return S_OK. Return 
   S_FALSE if we don't have the focus.

**************************************************************************/

STDMETHODIMP CIEPlus::HasFocusIO(void)
{
//	MessageBox(m_hWnd,"HasFocusIO","info",MB_OK|MB_ICONINFORMATION);
	if(m_bFocus)
		return S_OK;
	
	return S_FALSE;
}

/**************************************************************************

   CIEPlus::TranslateAcceleratorIO()
   
   If the accelerator is translated, return S_OK or S_FALSE otherwise.

**************************************************************************/

STDMETHODIMP CIEPlus::TranslateAcceleratorIO(LPMSG pMsg)
{
//  version 3
	switch(pMsg->wParam)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_HOME:
	case VK_END:
		SendMessage(m_hedit,pMsg->message,pMsg->wParam,pMsg->lParam);
		return S_OK;
	case VK_RETURN:
		if(WM_CHAR==pMsg->message)
		{
			TCHAR buf[250];
			::GetWindowText(m_hCombo,buf,250);
			m_pSE->DoSearch(m_pSE->GetDefault(),buf);
			return S_OK;
		}
		else
			break;
	case VK_BACK:
		if(WM_KEYDOWN==pMsg->message)
			SendMessage(m_hedit,WM_CHAR,pMsg->wParam,pMsg->lParam);
		return S_OK;
	default:
//
//		SendMessage(m_hedit,pMsg->message,pMsg->wParam,pMsg->lParam);
//		return S_OK;
		break;
	}

/*	version 2
	if(VK_LEFT==pMsg->wParam||VK_RIGHT==pMsg->wParam
		||VK_UP==pMsg->wParam||VK_DOWN==pMsg->wParam)
	{
		SendMessage(m_hedit,pMsg->message,pMsg->wParam,pMsg->lParam);
		return S_OK;
	}
	if(VK_RETURN==pMsg->wParam && WM_CHAR==pMsg->message)
	{
		TCHAR buf[250];
		::GetWindowText(m_hCombo,buf,250);
		m_pSE->DoSearch(m_pSE->GetDefault(),buf);
		return S_OK;
	}
	if(VK_BACK==pMsg->wParam)
	{
		if(WM_KEYUP==pMsg->message)
			SendMessage(m_hedit,WM_CHAR,pMsg->wParam,pMsg->lParam);
		return S_OK;
	}
*/
/*	version 1
	static int i=0;
	if(VK_BACK==pMsg->wParam||VK_LEFT==pMsg->wParam
		||VK_RIGHT==pMsg->wParam||VK_UP==pMsg->wParam
		||VK_DOWN==pMsg->wParam)
	{
		if(VK_BACK==pMsg->wParam)
		{
			i++;
//			SendMessage(m_hedit,WM_KEYDOWN,pMsg->wParam,pMsg->lParam);
			if(i==2)
			{
				SendMessage(m_hedit,WM_CHAR,pMsg->wParam,pMsg->lParam);
				i=0;
			}
//			SendMessage(m_hedit,WM_KEYUP,pMsg->wParam,pMsg->lParam);
		}
		else
			SendMessage(m_hedit,pMsg->message,pMsg->wParam,pMsg->lParam);
		return S_OK;
	}
*/	
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////
//
// IObjectWithSite implementations
//

/**************************************************************************

   CIEPlus::SetSite()
   
**************************************************************************/

STDMETHODIMP CIEPlus::SetSite(IUnknown* punkSite)
{
	HRESULT hr;
	//If a site is being held, release it.
	if(m_pSite)
	{
		m_pSite->Release();
		m_pSite = NULL;
	}

	//If punkSite is not NULL, a new site is being set.
	if(punkSite)
	{
		//Get the parent window.
		IOleWindow  *pOleWindow;

		m_hwndParent = NULL;
   
		if(SUCCEEDED(punkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
		{
			pOleWindow->GetWindow(&m_hwndParent);
			pOleWindow->Release();
		}

		if(!m_hwndParent)
		return E_FAIL;
			
		//Get and keep the IInputObjectSite pointer.
		if(SUCCEEDED(punkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&m_pSite)))
		{
      //
      // Get the IWebBrowser2 interface of Internet Explorer
      // This is so we can do such things as navigate in the main
      // window and write to the status bar.
      //
			IOleCommandTarget* pCmdTarget;
			hr = punkSite->QueryInterface(IID_IOleCommandTarget,
                                    (LPVOID*)&pCmdTarget);
			if (SUCCEEDED(hr))
			{
				IServiceProvider* pSP;
				hr = pCmdTarget->QueryInterface(IID_IServiceProvider,
                                         (LPVOID*)&pSP);

				pCmdTarget->Release();

				if (SUCCEEDED(hr))
				{
					if (m_pFrameWB)
					{
						m_pFrameWB.Release();
						m_pFrameWB = NULL;
					}

					hr = pSP->QueryService(SID_SWebBrowserApp,
                                   IID_IWebBrowser2,
                                   (LPVOID*)&m_pFrameWB);

					pSP->Release();
					ATLASSERT(SUCCEEDED(hr));
					AdviseEvent();
					if(FAILED(hr) || !RegisterAndCreateWindow())
						return E_FAIL;
				}
			}			
			return S_OK;
		}
   
		return E_FAIL;
	}

	return S_OK;
}

/**************************************************************************

   CIEPlus::GetSite()
   
**************************************************************************/

STDMETHODIMP CIEPlus::GetSite(REFIID riid, LPVOID *ppvReturn)
{
	*ppvReturn = NULL;

	if(m_pSite)
		return m_pSite->QueryInterface(riid, ppvReturn);

	return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////
//
// IDeskBand implementation
//

/**************************************************************************

   CIEPlus::GetBandInfo()
   
**************************************************************************/

STDMETHODIMP CIEPlus::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
	if(pdbi)
	{
		m_dwBandID = dwBandID;
		m_dwViewMode = dwViewMode;

		if(pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = 200;
			pdbi->ptMinSize.y = TB_MIN_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = 2000;
			pdbi->ptMaxSize.y = TB_MAX_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1;
			pdbi->ptIntegral.y = 1;
		}

		if(pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 600;
			pdbi->ptActual.y = TB_MAX_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_TITLE)
		{
			lstrcpyW(pdbi->wszTitle, L"");
		}

		if(pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_NORMAL;

			pdbi->dwModeFlags |= DBIMF_VARIABLEHEIGHT;
		}
   
		if(pdbi->dwMask & DBIM_BKCOLOR)
		{
		//Use the default background color by removing this flag.
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}
		return S_OK;
	}

	return E_INVALIDARG;
}

///////////////////////////////////////////////////////////////////////////
//
// IPersistStream implementations
// 
// This is only supported to allow the desk band to be dropped on the 
// desktop and to prevent multiple instances of the desk band from showing 
// up in the context menu. This desk band doesn't actually persist any data.
//

/**************************************************************************

   CIEPlus::GetClassID()
   
**************************************************************************/

STDMETHODIMP CIEPlus::GetClassID(LPCLSID pClassID)
{
	*pClassID = CLSID_IEPlus;

	return S_OK;
}

/**************************************************************************

   CIEPlus::IsDirty()
   
**************************************************************************/

STDMETHODIMP CIEPlus::IsDirty(void)
{
	return S_FALSE;
}

/**************************************************************************

   CIEPlus::Load()
   
**************************************************************************/

STDMETHODIMP CIEPlus::Load(LPSTREAM pStream)
{
	return S_OK;
}

/**************************************************************************

   CIEPlus::Save()
   
**************************************************************************/

STDMETHODIMP CIEPlus::Save(LPSTREAM pStream, BOOL fClearDirty)
{
	return S_OK;
}

/**************************************************************************

   CIEPlus::GetSizeMax()
   
**************************************************************************/

STDMETHODIMP CIEPlus::GetSizeMax(ULARGE_INTEGER *pul)
{
	return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
//   CIEPlus::GetTypeInfoCount()
//   
///////////////////////////////////////////////////////////////////////////

STDMETHODIMP CIEPlus::GetTypeInfoCount(UINT* pctinfo)
{
   return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
//   CIEPlus::GetTypeInfo()
//   
///////////////////////////////////////////////////////////////////////////

STDMETHODIMP CIEPlus::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
   return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
//   CIEPlus::GetIDsOfNames()
//   
///////////////////////////////////////////////////////////////////////////

STDMETHODIMP CIEPlus::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
                                           LCID lcid,DISPID* rgDispId)
{
   return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////
//
//   CIEPlus::Invoke()
//   
///////////////////////////////////////////////////////////////////////////

STDMETHODIMP CIEPlus::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                                    DISPPARAMS* pDispParams, VARIANT* pVarResult,
                                    EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
   if (IID_NULL != riid)
      return DISP_E_UNKNOWNINTERFACE;
   if (!pDispParams)
      return DISP_E_PARAMNOTOPTIONAL;
	static BOOL bClicked=FALSE;
	if(m_pPlugMgr!=NULL)
		m_pPlugMgr->Call(dispIdMember,pDispParams);
   switch (dispIdMember)
   {
	case DISPID_NEWWINDOW2:	
		{
//			BSTR tmp;
//			m_pFrameWB->get_LocationURL(&tmp);
//			MessageBox(m_hWnd,_bstr_t(tmp),"DISPID_NEWWINDOW",MB_OK|MB_ICONINFORMATION);
//			*(pDispParams->rgvarg[0].pboolVal)=VARIANT_TRUE;
			if(VARIANT_TRUE==*(pDispParams->rgvarg[0].pboolVal))
			{
				*(pDispParams->rgvarg[0].pboolVal)=VARIANT_FALSE;
				m_nClosed++;
				g_bCloseCurIE=TRUE;
				if(TRUE==m_bBeep)	MessageBeep((UINT)MB_ICONHAND);
				break;
			}
			if(FALSE==m_bAutoClose)	break;
			SHORT state1=GetKeyState(VK_SHIFT);
			SHORT state2=GetKeyState(VK_CONTROL);
			if( 0xFF00==(state1&0xFF00) || 0xFF00==(state2&0xFF00) ||
				TRUE==bClicked)
			{
//				*(pDispParams->rgvarg[0].pboolVal)=VARIANT_FALSE;
				bClicked=FALSE;
			}
			else
			{
				char buf[100];
				g_bCloseCurIE=TRUE;
				if(TRUE==m_bBeep)	MessageBeep((UINT)-1);
				m_nClosed++;
				if(m_bAutoClose)
				{
					_stprintf(buf,_T("已关闭%d个广告窗口"),m_nClosed);
					TBBUTTONINFO tbi;
					tbi.cbSize=sizeof(TBBUTTONINFO);
					tbi.dwMask=TBIF_TEXT;
					tbi.idCommand=2 + IDM_BUTTONSTART;
					tbi.pszText=buf;
					tbi.cchText=_tcslen(buf);
					SendMessage(m_hwndToolbar,TB_SETBUTTONINFO,2 + IDM_BUTTONSTART,(DWORD)&tbi);
				}
			}
			break;
		}
	case DISPID_BEFORENAVIGATE2:
		bClicked=FALSE;
//		ManageAnchorsEventSink(Unadvise);
		break;
	case DISPID_NAVIGATECOMPLETE2:
//		BSTR tmp;
//		m_pFrameWB->get_LocationURL(&tmp);
//		MessageBox(m_hWnd,_bstr_t(tmp),"NewWindow",MB_OK|MB_ICONINFORMATION);
		if(g_bCloseCurIE)
		{
			g_bCloseCurIE=!g_bCloseCurIE;
			PostMessage(m_hWebBrower,WM_CLOSE,0,0);
/*
			CComPtr<IDispatch> pDisp;
			CComPtr<IHTMLDocument2> pDoc;
			if (SUCCEEDED(m_pFrameWB->get_Document(&pDisp)) && pDisp)
			{
				HRESULT hr = pDisp->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc);
				pDisp.Release();
				if (SUCCEEDED(hr))
				{
			SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			VARIANT *param;
			bstr_t bsData((LPCTSTR)_T("<SCRIPT LANGUAGE=\"JScript\">\
window.close();\
</SCRIPT>\
"));
			hr = SafeArrayAccessData(psa, (LPVOID*)&param);
			param->vt = VT_BSTR;
			param->bstrVal = (BSTR)bsData;
			hr = pDoc->write(psa);
			hr = pDoc->close();
			hr = SafeArrayUnaccessData(psa);
				}
			}
/*/
		}
//		ManageAnchorsEventSink(Advise);
		break;
	case DISPID_HTMLELEMENTEVENTS_ONCLICK:
		bClicked=TRUE;
		break;
	default:
		return DISP_E_MEMBERNOTFOUND;
   }
   return S_OK;
}

void CIEPlus::ManageAnchorsEventSink(AdviseType adviseType)
{
	if (adviseType == Unadvise && _stackAnchorCookies.empty())
	{
		return;
	}

	if (m_pFrameWB)
	{
		// Sink Anchor Events
		IDispatch* pDisp;
		if (SUCCEEDED(m_pFrameWB->get_Document(&pDisp)) && pDisp)
		{
			IHTMLDocument2* pDoc;
			HRESULT hr = pDisp->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc);
			pDisp->Release();

			if (FAILED(hr))
				return;

			//
			// Advise all the anchors on the page so we can get the onclick events
			// For the search pages, the anchors collection is empty.  Therefore,
			// we have to iterate through the entire all collection and advise
			// each anchor tag.
			//
			IHTMLElementCollection* pElemColl;
			hr = pDoc->get_all(&pElemColl);
			pDoc->Release();

			if (FAILED(hr))
				return;

			long lNumElems = 0;
			pElemColl->get_length(&lNumElems);

			for (int i = 0; i < lNumElems; i++)
			{
				_variant_t vtItem((long)i), vtEmpty;
				hr = pElemColl->item(vtItem, vtEmpty, &pDisp);

				if (FAILED(hr))
					break;

				// Get the IHTMLElement interface
				IHTMLElement* pElem;

				hr = pDisp->QueryInterface(IID_IHTMLElement, (LPVOID*)&pElem);
				pDisp->Release();

				if (FAILED(hr))
					break;

				BSTR bstr;
				hr = pElem->get_tagName(&bstr);

				if (SUCCEEDED(hr))
				{
					_bstr_t bstrTagName(bstr);

					if (!lstrcmpi(bstrTagName, "A"))
					{
						
						LPCONNECTIONPOINT pCP = NULL;

						if (SUCCEEDED(GetConnectionPoint(pElem, DIID_HTMLAnchorEvents, &pCP)))
						{
							if (adviseType == Advise)
							{  
								DWORD dwCookie;

								// Connect the event sink
								hr = pCP->Advise(static_cast<IDispatch*>(this), &dwCookie);

								if (SUCCEEDED(hr))
									_stackAnchorCookies.push(dwCookie);

								pCP->Release();

								CComPtr<IHTMLAnchorElement> pAnch;
								if(SUCCEEDED(pElem->QueryInterface(IID_IHTMLAnchorElement,(LPVOID*)&pAnch)))
								{
									BSTR tmp;
									pAnch->get_href(&tmp);
				//					MessageBox(m_hWnd,_bstr_t(tmp),"Info",MB_OK|MB_ICONINFORMATION);
								}

							}
							else
							{
								if (!_stackAnchorCookies.empty())
								{
									// Disconnect the event sink
									hr = pCP->Unadvise(_stackAnchorCookies.top());
									pCP->Release();

									_stackAnchorCookies.pop();
								}
							}
						}
					}
				}

				pElem->Release();
			}

			pElemColl->Release();
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
// private method implementations
//
BOOL CIEPlus::RegisterAndCreateWindow()
{
	if(!m_hWnd)
	{
		if(!m_hwndParent)
			return FALSE;

		RECT  rc;
		GetClientRect(m_hwndParent, &rc);
		
		//If the window class has not been registered, then do so.

		WNDCLASS wc;
/*
		if(!GetClassInfo(g_hInst, TB_CLASS_NAME, &wc))
		{
			ZeroMemory(&wc, sizeof(wc));
			wc.style          = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
			wc.lpfnWndProc    = (WNDPROC)WndProc;
			wc.cbClsExtra     = 0;
			wc.cbWndExtra     = 0;
			wc.hInstance      = g_hInst;
			wc.hIcon          = NULL;
			wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
			wc.lpszMenuName   = NULL;
			wc.lpszClassName  = TB_CLASS_NAME;
      
			if(!RegisterClass(&wc))
			{
				//If RegisterClass fails, CreateWindow below will fail.
			}
		}
		GetClassInfo(g_hInst,TOOLBARCLASSNAME,&wc);
		orgProc=(DWORD)wc.lpfnWndProc;
		//Create the window. The WndProc will set m_hWnd.
		CreateWindowEx(   0,
                     TB_CLASS_NAME,
                     NULL,
                     WS_CHILD ,
//                     rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,
					 0,0,0,0,
                     m_hwndParent,
                     NULL,
                     g_hInst,
                     (LPVOID)this);
/*/
		INITCOMMONCONTROLSEX cct;
		cct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		cct.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_USEREX_CLASSES;
		if(FALSE == InitCommonControlsEx(&cct))
			return FALSE;
		GetClassInfo(g_hInst,TOOLBARCLASSNAME,&wc);
		orgProc2=(DWORD)wc.lpfnWndProc;
		wc.lpfnWndProc=WndProc2;
		wc.lpszClassName=TB_CLASS_NAME;
		if(!RegisterClass(&wc))
		{
			//If RegisterClass fails, CreateWindow below will fail.
		}
		/*

		HWND tmp=CreateWindowEx(0,TOOLBARCLASSNAME,NULL,WS_CHILD,
						rc.left,
						rc.top,
						rc.right - rc.left,
						rc.bottom - rc.top,
						m_hwndParent,NULL,g_hInst,(LPVOID)this);	
		orgProc=SetClassLong(tmp,GCL_WNDPROC,(LONG)WndProc);
		DestroyWindow(tmp);
//*/
/*	
		CreateWindowEx(0,TOOLBARCLASSNAME,NULL,CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE|WS_CHILD|WS_CLIPSIBLINGS|TBSTYLE_TRANSPARENT|TBSTYLE_FLAT|TBSTYLE_LIST,
						rc.left,
						rc.top,
						rc.right - rc.left,
						rc.bottom - rc.top,
//						0,0,0,0,
						m_hwndParent,NULL,g_hInst,(LPVOID)this);	
/*/
		CreateToolBar();
		Init();
		orgProc=SetWindowLong(m_hwndParent,GWL_WNDPROC,(LONG)WndProc);
		SetWindowLong(m_hwndParent,GWL_USERDATA,(LONG)this);		
	}

	ShowWindow(m_hWnd,SW_HIDE);
	return (m_hWnd!=NULL);	
//	return TRUE;
}

LRESULT CALLBACK CIEPlus::EditProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
//	static DWORD orgep=0;
//	if(NULL==g_pIEPlus)	return 1;
	CIEPlus* pThis=(CIEPlus*)GetWindowLong(hWnd, GWL_USERDATA);
	switch(uMessage)
	{
	case WM_SETFOCUS:
		pThis->OnSetFocus();
		break;
	case WM_KILLFOCUS:
		pThis->OnKillFocus();
		break;
//	case WM_DESTROY:
//		CallWindowProc((WNDPROC)g_pIEPlus->orgEditProc,hWnd,uMessage,wParam,lParam);
//		SetWindowLong(g_pIEPlus->m_hedit,GWL_WNDPROC,(LONG)g_pIEPlus->orgEditProc);
//		orgep=g_pIEPlus->orgEditProc;
//		ReportErrMsg();
//		return 1;
//	case WM_PAINT:

//		break;
	default:
		break;
	}
//	if(orgep)	return CallWindowProc((WNDPROC)orgep,hWnd,uMessage,wParam,lParam);
	return CallWindowProc((WNDPROC)pThis->orgEditProc,hWnd,uMessage,wParam,lParam);
}

/**************************************************************************

   CIEPlus::WndProc()
   
**************************************************************************/

LRESULT CALLBACK CIEPlus::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	CIEPlus  *pThis = (CIEPlus*)GetWindowLong(hWnd, GWL_USERDATA);
	switch (uMessage)
	{
	case WM_COMMAND:
		pThis->OnCommand(wParam, lParam);
		break;
	case WM_NOTIFY:
		pThis->OnNotify(wParam,lParam);
		break;						
	}
//	return DefWindowProc(hWnd, uMessage, wParam, lParam);
	return CallWindowProc((WNDPROC)pThis->orgProc,hWnd,uMessage,wParam,lParam);
}

LRESULT CALLBACK CIEPlus::WndProc2(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	CIEPlus  *pThis = (CIEPlus*)GetWindowLong(hWnd, GWL_USERDATA);
	switch (uMessage)
	{
	case WM_NCCREATE:
		{
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			pThis = (CIEPlus*)(lpcs->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, (LONG)pThis);
			//set the window handle
			pThis->m_hWnd = hWnd;
		}
		break;
/*
	case WM_COMMAND:
		pThis->OnCommand(wParam, lParam);
		break;
	case WM_NOTIFY:
		pThis->OnNotify(wParam,lParam);
		break;
*/		
	case WM_SETFOCUS:
				pThis->OnSetFocus();
				break;
	case WM_KILLFOCUS:
				 pThis->OnKillFocus();
				 break;
	case WM_SIZE:
//			pThis->OnSize(lParam);
			break;
	case WM_DESTROY:
				pThis->OnDestroy();
				break;
	case WM_IENHEXT_CLOSESESSION:
		pThis->m_pPlugMgr->CloseSession((int)wParam);
		return 0;
	}
	//	return DefWindowProc(hWnd, uMessage, wParam, lParam);
	return CallWindowProc((WNDPROC)pThis->orgProc2,hWnd,uMessage,wParam,lParam);
}

LRESULT CIEPlus::OnCreate()
{
/*		INITCOMMONCONTROLSEX cct;
		cct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		cct.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
		if(FALSE == InitCommonControlsEx(&cct))
			return FALSE;
*/
	CreateToolBar();
	Init();
	return 0;
}

/**************************************************************************

   CIEPlus::OnCommand()
   
**************************************************************************/

LRESULT CIEPlus::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT id = (UINT)wParam;
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU sub;
//  代理服务器管理
	if( id <= ID_MENUSTART+MAX_MENU_ENTRY && id >= ID_MENUSTART)
	{
		GetMenuItemInfo(m_HttpMenu,id,FALSE,&mii);
		if(MF_CHECKED == (mii.fState & MF_CHECKED))
		{
			m_pPM->SetProxy(NULL,NULL,_T("http"));
			CheckMenuItem(m_HttpMenu,id,MF_BYCOMMAND|MF_UNCHECKED);
			m_nSel = -1;
		}
		else if(MF_UNCHECKED == (mii.fState & MF_UNCHECKED))
		{
			m_pPM->SetProxy(m_HttpList[id-ID_MENUSTART].name.c_str(),
					m_HttpList[id-ID_MENUSTART].port.c_str(),
					m_HttpList[id-ID_MENUSTART].type.c_str());
			CheckMenuItem(m_HttpMenu,id,MF_BYCOMMAND|MF_CHECKED);
			if(-1 != m_nSel)
				CheckMenuItem(m_HttpMenu,m_nSel+ID_MENUSTART,MF_BYCOMMAND|MF_UNCHECKED);
			m_nSel = id-ID_MENUSTART;
		}
		return 0;
	}

//  使用搜索引擎
	if( id >= ID_SEMENUSTART && id < ID_SEMENUSTART+m_pSE->GetSize())
	{
		TCHAR buf[250];
		::GetWindowText(m_hCombo,buf,250);
		m_pSE->DoSearch(id-ID_SEMENUSTART,buf);
		return 0;
	}

//	plugin selected
	if(id>=IDM_PLUGIN_BASE && id<IDM_PLUGIN_BASE+m_pPlugMgr->GetSize())
	{
		m_pPlugMgr->SetSessionActive(id,(m_pPlugMgr->Execute(id))
				(m_hWnd,m_hWebBrower,(void*)m_pFrameWB.p,id));
		return 0;
	}

	switch(id)
	{
	case IDM_BUTTONSTART:
		break;
	case 1 + IDM_BUTTONSTART:
		break;
	case 2 + IDM_BUTTONSTART:
		m_bAutoClose=(0==::SendMessage(m_hwndToolbar,TB_ISBUTTONCHECKED,2+IDM_BUTTONSTART,0)) ? FALSE : TRUE;

		char buf[100];
		if(m_bAutoClose)
			_stprintf(buf,_T("已关闭%d个广告窗口"),m_nClosed);
		else
			_stprintf(buf,_T("自动关闭广告窗口"));
		TBBUTTONINFO tbi;
		tbi.cbSize=sizeof(TBBUTTONINFO);
		tbi.dwMask=TBIF_TEXT;
		tbi.idCommand=2 + IDM_BUTTONSTART;
		tbi.pszText=buf;
		tbi.cchText=_tcslen(buf);
		SendMessage(m_hwndToolbar,TB_SETBUTTONINFO,2 + IDM_BUTTONSTART,(DWORD)&tbi);
		
		break;
	case 3 + IDM_BUTTONSTART:		// 快速保存
		{
			CQuickSaveDlg dlg(m_pFrameWB);
			dlg.DoModal();
			break;
		}
	case 5 + IDM_BUTTONSTART:		// 搜索引擎
		{
			TCHAR buf[250];
			::GetWindowText(m_hCombo,buf,250);
			m_pSE->DoSearch(-1,buf);
			break;
		}
	case IDR_USEPROXY:				
		{
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STATE;
			GetMenuItemInfo(m_PopupMenu,IDR_USEPROXY,FALSE,&mii);
			if(mii.fState & MF_CHECKED)
			{
				m_pPM->EnableProxy(FALSE);
				mii.fState = MF_UNCHECKED;
			}
			else
			{
				m_pPM->EnableProxy();
				mii.fState = MF_CHECKED;
			}
			SetMenuItemInfo(m_PopupMenu,IDR_USEPROXY,FALSE,&mii);
			break;
		}
	case ID_MOREWND:
			if(!ShowMoreWnd())
				ReportErrMsg();
			break;
	case ID_CONFIGPROXY:
		{
			COptionData a(m_pSE,m_pPM,m_bBeep,m_nComboSize);
			COptionDlg dlg(1,a);
			dlg.DoModal(m_hWebBrower);
			break;
		}
	case ID_OPTION:
		{
			COptionData a(m_pSE,m_pPM,m_bBeep,m_nComboSize);
			COptionDlg dlg(0,a);
			dlg.DoModal(m_hWebBrower);
			TBBUTTONINFO bti;
			bti.cbSize=sizeof(bti);
			bti.dwMask=TBIF_SIZE;
			bti.cx=m_nComboSize;
			SendMessage(m_hwndToolbar,TB_SETBUTTONINFO,4+IDM_BUTTONSTART,(LPARAM)&bti);
			::MoveWindow(m_hCombo,m_nComboOrd,0,m_nComboSize-5,IMG_HEIGHT,TRUE);	
			break;
		}
	case ID_ABOUT:
		{
//			COptionData a(m_pSE,m_pPM);
//			COptionDlg dlg(2,a);
//			dlg.DoModal(m_hWebBrower);
			variant_t vtEmpty;
			tstring file(GD_MODULE_PATH);
			file+=_T("html\\about.htm");
			m_pFrameWB->Navigate(bstr_t(file.c_str()),&vtEmpty,&vtEmpty,&vtEmpty,&vtEmpty);
			break;
		}
	//  下载限制:
	case ID_LIMITIMG:
		{
			sub = GetSubMenu(m_MainMenu,0);
			GetMenuItemInfo(sub,id,FALSE,&mii);
			if(MF_CHECKED == (mii.fState & MF_CHECKED))
			{
				DisplayImage();
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_UNCHECKED);
			}
			else if(MF_UNCHECKED == (mii.fState & MF_UNCHECKED))
			{
				DisplayImage(FALSE);
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_CHECKED);
			}
			break;
		}
	case ID_LIMITVIDEO:
		{
			sub = GetSubMenu(m_MainMenu,0);
			GetMenuItemInfo(sub,id,FALSE,&mii);
			if(MF_CHECKED == (mii.fState & MF_CHECKED))
			{
				DisplayVideo();
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_UNCHECKED);
			}
			else if(MF_UNCHECKED == (mii.fState & MF_UNCHECKED))
			{
				DisplayVideo(FALSE);
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_CHECKED);
			}
			break;
		}
	case ID_LIMITANIM:
		{
			sub = GetSubMenu(m_MainMenu,0);
			GetMenuItemInfo(sub,id,FALSE,&mii);
			if(MF_CHECKED == (mii.fState & MF_CHECKED))
			{
				DisplayAnim();
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_UNCHECKED);
			}
			else if(MF_UNCHECKED == (mii.fState & MF_UNCHECKED))
			{
				DisplayAnim(FALSE);
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_CHECKED);
			}
			break;
		}
	case ID_NOTPLAYSOUND:
		{
			sub = GetSubMenu(m_MainMenu,0);
			GetMenuItemInfo(sub,id,FALSE,&mii);
			if(MF_CHECKED == (mii.fState & MF_CHECKED))
			{
				PlaySound();
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_UNCHECKED);
			}
			else if(MF_UNCHECKED == (mii.fState & MF_UNCHECKED))
			{
				PlaySound(FALSE);
				CheckMenuItem(sub,id,MF_BYCOMMAND|MF_CHECKED);
			}
			break;
		}
	default:
			break;
	}
	return 0;
}

LRESULT CIEPlus::OnNotify(WPARAM wParam, LPARAM lParam)
{
#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

   RECT      rc;
   TPMPARAMS tpm;

  static TCHAR buf[MAX_TEXT_LENGTH/10];
   switch(lpnm->code){
      case TBN_DROPDOWN:
		{
		SendMessage(lpnmTB->hdr.hwndFrom, TB_GETRECT,
	            (WPARAM)lpnmTB->iItem, (LPARAM)&rc);

		MapWindowPoints(lpnmTB->hdr.hwndFrom,
                HWND_DESKTOP, (LPPOINT)&rc, 2);                         

		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		switch(lpnmTB->iItem)
		  {
		  case IDM_BUTTONSTART:
			  {
					TrackPopupMenuEx(m_MainMenu,
						TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,               
						rc.left, rc.bottom, m_hwndParent, &tpm); 
					break;
			  }
		  case 1 + IDM_BUTTONSTART:
			  {
					ShowProxyPopup(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
									rc.left, rc.bottom,&tpm);
					break;
			  }
		  case 5 + IDM_BUTTONSTART:
			  {
					ShowSePopup(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
									rc.left, rc.bottom,&tpm);
					break;
			  }
		  }
		}
	/*
        case TTN_GETDISPINFO:        // XP下不工作,ft
            { 
            LPTOOLTIPTEXT lpttt; 
            lpttt = (LPTOOLTIPTEXT) lParam; 
            lpttt->hinst = g_hInst; 
            // Specify the resource identifier of the descriptive 
            // text for the given button. 
            int idButton = lpttt->hdr.idFrom; 
            switch (idButton) 
			{ 
                case 1 + IDM_BUTTONSTART: 
                    lpttt->lpszText = _T("代理服务器管理"); 
                    break; 
				case 2 + IDM_BUTTONSTART:
					if(TRUE==m_bAutoClose)
						_stprintf(buf,_T("已关闭%d个广告窗口"),m_nClosed);
					else
						_tcscpy(buf,_T("自动关闭广告窗口"));
					lpttt->lpszText = buf;
					break;
                case 3 + IDM_BUTTONSTART: 
                    lpttt->lpszText = _T("提取Flash和图片"); 
                    break; 
				case 5 + IDM_BUTTONSTART:
					lpttt->lpszText = _T("搜索");
					break;
            } 
		}

		case TBN_GETINFOTIP:         // 不设置TBSTYLE_EX_MIXEDBUTTON居然不响应, ft
			{
				LPNMTBGETINFOTIP lptbgit = (LPNMTBGETINFOTIP) lParam; 
				switch(lptbgit->iItem)
				{
				case 2 + IDM_BUTTONSTART:
					if(TRUE==m_bAutoClose)
						_stprintf(buf,_T("已关闭%d个广告窗口"),m_nClosed);
					else
						_tcscpy(buf,_T("自动关闭广告窗口"));
					lptbgit->pszText = buf;
					lptbgit->cchTextMax=_tcslen(buf);
					break;
				}
			}
		*/
   }
   return FALSE;
}

/**************************************************************************

   CIEPlus::OnSize()
   
**************************************************************************/

LRESULT CIEPlus::OnSize(LPARAM lParam)
{
	int   cx, cy;

	cx = LOWORD(lParam);
	cy = HIWORD(lParam);
	MoveWindow(m_hwndToolbar,0,0,cx,cy,FALSE);
//	MoveWindow(m_hWnd,0,0,cx,cy,FALSE);
//	RedrawWindow(m_hwndToolbar,NULL,NULL,RDW_INTERNALPAINT);
//	RedrawWindow(m_hWnd,NULL,NULL,RDW_INTERNALPAINT);
//	SetWindowLong(m_hWnd,GWL_EXSTYLE,0);
//	SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_TRANSPARENT);
	return 0;
}

/**************************************************************************

   CIEPlus::FocusChange()
   
**************************************************************************/

void CIEPlus::FocusChange(BOOL bFocus)
{
	m_bFocus = bFocus;

	//inform the input object site that the focus has changed
	if(m_pSite)
	{
		m_pSite->OnFocusChangeIS((IDockingWindow*)this, bFocus);
	}
}

/**************************************************************************

   CIEPlus::OnSetFocus()
   
**************************************************************************/

LRESULT CIEPlus::OnSetFocus(void)
{
	FocusChange(TRUE);	
	return 0;
}

/**************************************************************************

   CIEPlus::OnKillFocus()
   
**************************************************************************/

LRESULT CIEPlus::OnKillFocus(void)
{
	FocusChange(FALSE);
	return 0;
}

void CIEPlus::AdviseEvent()
{
   if (m_pFrameWB)
   {
      LPCONNECTIONPOINT pCP = NULL;
      // Sink WebBrowser Events
      if (SUCCEEDED(GetConnectionPoint(m_pFrameWB, DIID_DWebBrowserEvents2, &pCP)))
      {
         pCP->Advise(static_cast<IDispatch*>(this), &m_dwWBCookie);
         pCP->Release();
      }
   }
}

void CIEPlus::UnadviseEvent()
{
   if (m_pFrameWB)
   {
      LPCONNECTIONPOINT pCP = NULL;
      // Unadvise the WebBrowser Event Sink 
      if (m_dwWBCookie && SUCCEEDED(GetConnectionPoint(m_pFrameWB, DIID_DWebBrowserEvents2, &pCP)))
      {
         pCP->Unadvise(m_dwWBCookie);
         pCP->Release();
         m_dwWBCookie=0;
      }
   }
}

HRESULT CIEPlus::GetConnectionPoint(LPUNKNOWN pUnk, REFIID riid, LPCONNECTIONPOINT* ppCP)
{
   HRESULT hr = E_FAIL;
   IConnectionPointContainer* pCPC;

   if (m_pFrameWB)
   {
      hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
      if (FAILED(hr))
         return hr;

      hr = pCPC->FindConnectionPoint(riid, ppCP);
      pCPC->Release();
   }
   return hr;
}

void CIEPlus::Init()		
{	
	m_PopupMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP)); 
	m_ProxyMenu = GetSubMenu(m_PopupMenu,1);
	m_MainMenu = GetSubMenu(m_PopupMenu,0);

	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HKEY key;
	DWORD disp;
	TCHAR szVal[100];
	TCHAR szBuf[20];
	DWORD type = REG_SZ;
	unsigned long size = 20;
	HMENU sub;
	sub = GetSubMenu(m_MainMenu,0);
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_IE_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
		return;
	_tcscpy(szVal,_T("Display Inline Images"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcscmp(szBuf,_T("no")))
			CheckMenuItem(sub,0,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(sub,0,MF_BYPOSITION|MF_UNCHECKED);
	}
	size = 20;
	_tcscpy(szVal,_T("Display Inline Videos"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcscmp(szBuf,_T("no")))
			CheckMenuItem(sub,1,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(sub,1,MF_BYPOSITION|MF_UNCHECKED);
	}
	size = 20;
	_tcscpy(szVal,_T("Play_Animations"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcscmp(szBuf,_T("no")))
			CheckMenuItem(sub,2,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(sub,2,MF_BYPOSITION|MF_UNCHECKED);
	}
	size = 20;
	_tcscpy(szVal,_T("Play_Background_Sounds"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcscmp(szBuf,_T("no")))
			CheckMenuItem(sub,3,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(sub,3,MF_BYPOSITION|MF_UNCHECKED);
	}
	RegCloseKey(key);

	m_bAutoClose=TRUE;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_MAIN_REG,
		0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
		NULL,&key,&disp))
		return;
	size = 20;
	_tcscpy(szVal,_T("AutoClose"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcsicmp(szBuf,_T("no")))
			m_bAutoClose=FALSE;
	}
	::SendMessage(m_hwndToolbar,TB_CHECKBUTTON,2+IDM_BUTTONSTART,MAKELONG(m_bAutoClose,0));
	char buf[100];
	if(m_bAutoClose)
		_stprintf(buf,_T("已关闭%d个广告窗口"),m_nClosed);
	else
		_stprintf(buf,_T("自动关闭广告窗口"));
	TBBUTTONINFO tbi;
	tbi.cbSize=sizeof(TBBUTTONINFO);
	tbi.dwMask=TBIF_TEXT;
	tbi.idCommand=2 + IDM_BUTTONSTART;
	tbi.pszText=buf;
	tbi.cchText=_tcslen(buf);
	SendMessage(m_hwndToolbar,TB_SETBUTTONINFO,2 + IDM_BUTTONSTART,(DWORD)&tbi);
				
	m_bBeep=TRUE;
	size = 20;
	_tcscpy(szVal,_T("Beep"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
	{
		if(0 == _tcsicmp(szBuf,_T("no")))
			m_bBeep=FALSE;
	}
	RegCloseKey(key);
	
	m_hWebBrower = m_hWnd;			// 获得顶层窗口
	HWND IEWnd2 = NULL;
	while(IsWindow(m_hWebBrower))
	{
		IEWnd2 = m_hWebBrower;
		m_hWebBrower = GetParent(m_hWebBrower);
	}
	m_hWebBrower = IEWnd2;

	// 出错信息很不完整
	if(!m_pPM)
	{
		m_pPM = new CProxyManager();
		if(FALSE == m_pPM->Init())
		{	
			MessageBox(m_hWnd,_T("Proxy Manager Error"),"Error",MB_OK);
			delete m_pPM;
			m_pPM = NULL;
			SendMessage(m_hWnd,WM_CLOSE,0,0);
		}	

		SetupRasListener();
	}
	if(!m_pSE)
	{
		m_pSE = new CSearchEngine(m_pFrameWB);
		if(FALSE == m_pSE->Init(m_hCombo))
		{
			MessageBox(m_hWnd,_T("Search Engine Init Error"),"Error",MB_OK);
			delete m_pSE;
			m_pSE = NULL;
			SendMessage(m_hWnd,WM_CLOSE,0,0);
		}
	}
	if(!m_pPlugMgr)
	{
		m_pPlugMgr=new CPluginManager;
		if(FALSE==m_pPlugMgr->Init(m_MainMenu))
		{
			MessageBox(m_hWnd,_T("Dll Init Error"),"Error",MB_OK|MB_ICONSTOP);
			delete m_pPlugMgr;
			m_pPlugMgr=NULL;
			SendMessage(m_hWnd,WM_CLOSE,0,0);
		}
	}
}


void CIEPlus::Store()
{
	HKEY key;
	DWORD disp;
	TCHAR szVal[100];
	TCHAR szBuf[20];
	DWORD type = REG_SZ;
	unsigned long size = 20;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_MAIN_REG,
		0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
		NULL,&key,&disp))
		return;
	size = 20;
	_tcscpy(szVal,_T("AutoClose"));
	if(FALSE==m_bAutoClose)
	{
		_tcscpy(szBuf,_T("no"));
		RegSetValueEx(key,szVal,NULL,type,(BYTE*)szBuf,_tcslen(szBuf));
	}
	else
	{
		_tcscpy(szBuf,_T("yes"));
		RegSetValueEx(key,szVal,NULL,type,(BYTE*)szBuf,_tcslen(szBuf));
	}

	size = 20;
	_tcscpy(szVal,_T("Beep"));
	if(FALSE==m_bBeep)
	{
		_tcscpy(szBuf,_T("no"));
		RegSetValueEx(key,szVal,NULL,type,(BYTE*)szBuf,_tcslen(szBuf));
	}
	else
	{
		_tcscpy(szBuf,_T("yes"));
		RegSetValueEx(key,szVal,NULL,type,(BYTE*)szBuf,_tcslen(szBuf));
	}
	
	_tcscpy(szVal,_T("ComboSize"));
	RegSetValueEx(key,szVal,NULL,REG_DWORD,(BYTE*)&m_nComboSize,sizeof(DWORD));
	RegCloseKey(key);

	// 出错信息未完成
	if(m_pPM && m_pPM->HasChanged())
	{
		if(FALSE == m_pPM->Store())
			MessageBox(m_hWnd,_T("Proxy Manager Error"),"Error",MB_OK);
		delete m_pPM;
	}
	if(m_pSE)
	{
		m_pSE->StoreTyped();
		if(m_pSE->HasChanged())
		{
			if(FALSE == m_pSE->Store())
				MessageBox(m_hWnd,_T("Search Engine Store Error"),"Error",MB_OK);
			delete m_pSE;
		}
	}
	if(m_pPlugMgr)
		delete m_pPlugMgr;
}

void CIEPlus::ShowProxyPopup(UINT flag,int x,int y,LPTPMPARAMS lptpm)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	mii.fState = MF_CHECKED;

	if(!m_pPM->IsProxyEnable())
		mii.fState = MF_UNCHECKED;
	SetMenuItemInfo(m_ProxyMenu,IDR_USEPROXY,FALSE,&mii);

	if(m_pPM->ReqFresh())
	{
		m_pPM->GetProxySettingFromIE();
		RefreshProxyMenu();		
	}

	TrackPopupMenuEx(m_ProxyMenu,flag,x,y,m_hWnd,lptpm);
}

void CIEPlus::RefreshProxyMenu()
{
	if(NULL != m_HttpMenu)
	{
		if(!DeleteMenu(m_ProxyMenu,3,MF_BYPOSITION))
			ReportErrMsg();
		m_HttpMenu = NULL;
	}
	if(m_HttpList.size())
		m_HttpList.clear();
	m_HttpMenu = CreatePopupMenu();
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE | MIIM_ID;
	mii.wID = 100;
	mii.fType = MFT_STRING;
	mii.cch = _tcslen(_T("HTTP 代理"));
	mii.dwTypeData = _T("HTTP 代理");
	mii.hSubMenu = m_HttpMenu;
	if(!InsertMenuItem(m_ProxyMenu,3,TRUE,&mii))
		ReportErrMsg();
	
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.fType = MFT_STRING;
	BOOL bFound = FALSE;
	tstring str;
	vector<ProxyEntry>& vec = m_pPM->GetProxyList();
	int size = vec.size();
	for(int i=0,j=0;i<size;i++)
	{
		if(0 != vec[i].type.compare(_T("http")))
			continue;
		if(j!=MAX_MENU_ENTRY)
		{
			m_HttpList.push_back(vec[i]);
			if(j<MAX_MENU_ENTRY)
			{	
				mii.wID = j + ID_MENUSTART;
				str.erase(0,str.size());
				str += vec[i].name;
				str += _T(':');
				str += vec[i].port;
//				str += _T('@');
//				str += vec[i].type;
				str += _T("  ");
				str += vec[i].comment;
				mii.cch = str.size();
				mii.dwTypeData =(TCHAR*)str.c_str();
				if(InsertMenuItem(m_HttpMenu,j,TRUE,&mii))
				{
					if(!bFound)
					{
						if(m_pPM->IsProxyEqual(m_HttpList[j].name.c_str(),m_HttpList[j].port.c_str(),COMPARE_HTTP))
						{
							bFound = TRUE;
							CheckMenuItem(m_HttpMenu,ID_MENUSTART+j,MF_BYCOMMAND|MF_CHECKED);
							m_nSel = j;
						}
					}
					j++;
				}
				else
					ReportErrMsg();
			}
		}
		else
		{
			mii.wID = ID_MOREWND;
			mii.cch = _tclen(_T("全部..."));
			mii.dwTypeData = _T("全部...");
			if(!InsertMenuItem(m_HttpMenu,j,TRUE,&mii))
				ReportErrMsg();
			j++;
		}
	}
	if(0 == i)		// 代理列表为空
	{
		mii.wID = ID_MENUSTART-1;
		mii.cch = _tclen(_T("没有代理"));
		mii.dwTypeData = _T("没有代理");
		mii.fMask |= MIIM_STATE;
		mii.fState = MFS_DISABLED;
		if(!InsertMenuItem(m_HttpMenu,0,TRUE,&mii))
			ReportErrMsg();
	}
	m_pPM->SetReqFresh(FALSE);
}

BOOL CIEPlus::IsWinXP()
{
//  Check OS Version
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
// If that fails, try using the OSVERSIONINFO structure.
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	BOOL bWinXP=FALSE;
	if(VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		bWinXP=TRUE;
	return bWinXP;
}

BOOL CIEPlus::CreateToolBar()
{
	RECT  rc;
	BOOL bWinXP=CIEPlus::IsWinXP();
	DWORD style=WS_CHILD|CCS_NODIVIDER|CCS_BOTTOM|CCS_NORESIZE;
//	if(FALSE==bWinXP)
		style|=TBSTYLE_FLAT|TBSTYLE_TRANSPARENT|TBSTYLE_LIST|TBSTYLE_TOOLTIPS;
	m_hwndToolbar = CreateWindowEx(0,TB_CLASS_NAME,NULL,style,
						0,0,0,0,m_hwndParent,NULL,g_hInst,this); //here changes |TBSTYLE_FLAT
//	m_hwndToolbar=m_hWnd;
	if(m_hwndToolbar==NULL)
		return FALSE;
/*
	SendMessage(m_hwndToolbar,TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON),0);
	SetWindowLong(m_hwndToolbar,GWL_WNDPROC,(LONG)WndProc2);	
	orgProc2=SetWindowLong(m_hwndToolbar,GWL_USERDATA,(LONG)this);
/*/	
//	himg = ImageList_Create(IMG_WIDTH,IMG_HEIGHT,ILC_COLOR8,0,4);		
//	himgcold = ImageList_Create(IMG_WIDTH,IMG_HEIGHT,ILC_COLOR8,0,4);
//	HBITMAP hbmphot,hbmpcold;

	if(TRUE==bWinXP)
	{
//		hbmphot = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_HOT));
//		hbmpcold = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_COLD));
		himg=ImageList_LoadImage(g_hInst,MAKEINTRESOURCE(IDB_HOT),IMG_WIDTH,4,CLR_NONE,IMAGE_BITMAP,LR_CREATEDIBSECTION|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
		himgcold=ImageList_LoadImage(g_hInst,MAKEINTRESOURCE(IDB_COLD),IMG_WIDTH,4,CLR_NONE,IMAGE_BITMAP,LR_CREATEDIBSECTION|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);

	}
	else
	{
//		hbmphot = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_HOTNT));
//		hbmpcold = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_COLDNT));
		himg=ImageList_LoadImage(g_hInst,MAKEINTRESOURCE(IDB_HOTNT),IMG_WIDTH,4,CLR_NONE,IMAGE_BITMAP,LR_CREATEDIBSECTION|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
		himgcold=ImageList_LoadImage(g_hInst,MAKEINTRESOURCE(IDB_COLDNT),IMG_WIDTH,4,CLR_NONE,IMAGE_BITMAP,LR_CREATEDIBSECTION|LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);
	}

//	ImageList_Add(himg,hbmphot,NULL);
//	ImageList_Add(himgcold,hbmpcold,NULL);
//	DeleteObject(hbmphot);
//	DeleteObject(hbmpcold);

//	COLORREF clr = ImageList_GetBkColor(himg);
//	clr = ImageList_GetBkColor(himgcold);
//	clr = GetSysColor(COLOR_WINDOWFRAME);
//	ImageList_SetBkColor(himg,RGB(0,255,0));
//	ImageList_SetBkColor(himgcold,RGB(0,255,0));
	// Delete the loaded bitmap.

	SendMessage(m_hwndToolbar,TB_SETHOTIMAGELIST,0,(LPARAM)himg);
	SendMessage(m_hwndToolbar,TB_SETIMAGELIST,0,(LPARAM)himgcold);

	HKEY key;
	DWORD disp;
	TCHAR szVal[100];
	DWORD buf;
	DWORD type = REG_DWORD;
	unsigned long sz = 20;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_MAIN_REG,
		0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
		NULL,&key,&disp))
		return FALSE;
	_tcscpy(szVal,_T("ComboSize"));
	if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)&buf,&sz))
	{
		m_nComboSize=(int)buf;
		if(m_nComboSize<MIN_COMBO_SIZE)	m_nComboSize=MIN_COMBO_SIZE;
		else if(m_nComboSize>MAX_COMBO_SIZE)	m_nComboSize=MAX_COMBO_SIZE;
	}
	else
		m_nComboSize=MIN_COMBO_SIZE;
	RegCloseKey(key);
	
	TBBUTTON tbArray[6];
	int i;
	// Loop to fill the array of TBBUTTON structures.  
	for(i=0;i<4;i++)
	{
		tbArray[i].iBitmap   = i; // Bitmap from image list.
		tbArray[i].idCommand = IDM_BUTTONSTART + i;
		tbArray[i].fsStyle   = TBSTYLE_BUTTON;
		tbArray[i].fsState   = TBSTATE_ENABLED;
		tbArray[i].dwData    = 0;
	}
	tbArray[0].fsStyle   = BTNS_WHOLEDROPDOWN|TBSTYLE_DROPDOWN|TBSTYLE_AUTOSIZE|BTNS_SHOWTEXT;
	tbArray[0].iString   = (int)"iEnhance";
	tbArray[1].fsStyle	 = BTNS_WHOLEDROPDOWN|TBSTYLE_DROPDOWN|TBSTYLE_AUTOSIZE;
	tbArray[1].iString   = (int)_T("代理服务器管理");
	tbArray[2].fsStyle	 = TBSTYLE_CHECK|TBSTYLE_AUTOSIZE;  // 第二个为自动关闭弹出窗口
	tbArray[2].fsState	 = TBSTATE_CHECKED|TBSTATE_ENABLED;
	tbArray[2].iString   = (int)0;			
	tbArray[3].fsStyle   = TBSTYLE_AUTOSIZE;  // 第三个为快速保存
	tbArray[3].iString   = (int)_T("提取Flash和图片");
	tbArray[4].iBitmap	= m_nComboSize;				// 第四个为组合框，宽度将来可调
	tbArray[4].idCommand = IDM_BUTTONSTART + 4;
	tbArray[4].fsState   = TBSTATE_ENABLED;
	tbArray[4].fsStyle   = TBSTYLE_SEP;
	tbArray[4].dwData    = 0;
	tbArray[4].iString   = 0;
	tbArray[5].iBitmap = 4;
	tbArray[5].idCommand = IDM_BUTTONSTART + 5;
	tbArray[5].fsStyle   = TBSTYLE_DROPDOWN|TBSTYLE_AUTOSIZE;
	tbArray[5].fsState   = TBSTATE_ENABLED;
	tbArray[5].dwData    = 0;
	tbArray[5].iString = (int)_T("搜索");

	SendMessage(m_hwndToolbar,TB_ADDBUTTONS,6,(LPARAM)tbArray);
	SendMessage(m_hwndToolbar,TB_SETEXTENDEDSTYLE,0,(LPARAM)TBSTYLE_EX_DRAWDDARROWS|TBSTYLE_EX_MIXEDBUTTONS);
	ShowWindow(m_hwndToolbar,SW_SHOW);
    UpdateWindow(m_hwndToolbar);

	TBBUTTONINFO tbi;
	ZeroMemory(&tbi,sizeof(tbi));
	WORD size=0;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_SIZE;
//	RECT rc;
	ZeroMemory(&rc,sizeof(rc));
	for(i=0;i<4;i++)
	{
		SendMessage(m_hwndToolbar,TB_GETITEMRECT,i,(LPARAM)&rc);
		size+=(WORD)(rc.right-rc.left);
	}
	HWND hcombo = CreateWindowEx(0,WC_COMBOBOXEX,NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL|CBS_AUTOHSCROLL,
						0,0,0,IMG_HEIGHT+120,m_hWnd,NULL,g_hInst,NULL);
	SetParent(hcombo,m_hwndToolbar);
	::MoveWindow(hcombo,size+5,0,m_nComboSize-5,IMG_HEIGHT,TRUE);	// 此处使用了将来可调地组合框宽度变量
	m_hCombo = hcombo;
	m_nComboOrd = size + 5;
	m_hedit=(HWND)SendMessage(m_hCombo,CBEM_GETEDITCONTROL,0,0);
//	POINT pt;	pt.x=pt.y=10;
//	m_hedit=ChildWindowFromPoint(m_hCombo,pt);
	if(NULL!=m_hedit)
	{
		orgEditProc=(DWORD)SetWindowLong(m_hedit,GWL_WNDPROC,(LONG)EditProc);
		SetWindowLong(m_hedit,GWL_USERDATA,(LONG)(LPVOID)this);
	}
	return TRUE;
}

BOOL CIEPlus::ShowMoreWnd()
{
	if(!m_pMoreWnd)
		m_pMoreWnd = new CMoreWnd(this);
	else if(m_pMoreWnd->IsWindow())
		return TRUE;
	RECT rc;
	::GetClientRect(m_hWebBrower,&rc);
	rc.left = rc.right/3;
	rc.top = rc.bottom/3;
	rc.right = rc.right/2+rc.left;
	rc.bottom = rc.bottom*2/5+rc.top;
//	MapWindowPoints(m_hWnd,HWND_DESKTOP, (LPPOINT)&rc, 2);  
	// 不用WS_CHILD，然后SetParent之后的效果!
	m_pMoreWnd->Create(NULL,rc,_T("More..."));
	m_pMoreWnd->SetParent(m_hWebBrower);
	return TRUE;
}

BOOL CIEPlus::OnDestroy()
{
	ImageList_Destroy(himg);
	ImageList_Destroy(himgcold);
	UnadviseEvent();
	if(m_pMoreWnd && m_pMoreWnd->IsWindow())
	{
		m_pMoreWnd->SendMessage(WM_CLOSE);
		delete m_pMoreWnd;
		m_pMoreWnd = NULL;
	}
	Store();
	SetWindowLong(m_hedit,GWL_WNDPROC,orgEditProc);
	SetWindowLong(m_hwndParent,GWL_WNDPROC,orgProc);
	return 0;
}

vector<ProxyEntry>& CIEPlus::GetProxyList(int type /* = 0 */)
{
	if(0 == type)
		return m_HttpList;
	return m_HttpList;
}


void CIEPlus::ShowSePopup(UINT flag, int x, int y, LPTPMPARAMS lptpm)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_TYPE| MIIM_ID ;
	mii.fType = MFT_STRING;

	int i;
	tstring str;
	int def = m_pSE->GetDefault();

	if(m_pSE->ReqFresh())
	{
		if(NULL != m_SEMenu)
		{
			if(!DeleteMenu(m_SEMenu,0,MF_BYPOSITION))
				ReportErrMsg();
		}
		m_SEMenu = CreatePopupMenu();
		for(i=0;i<m_pSE->GetSize();i++)
		{
			mii.wID = i + ID_SEMENUSTART;
			str = m_pSE->GetName(i);
			mii.cch = str.size() ;
			if(i == def)
			{
				mii.fMask |= MIIM_STATE;
				mii.fState = MFS_DEFAULT;
			}
			else
				mii.fMask &= ~MIIM_STATE;
			mii.dwTypeData =(TCHAR*)(str.c_str());
			if(!InsertMenuItem(m_SEMenu,i,TRUE,&mii))
			{
			}
		}
		m_pSE->SetReqFresh();
	}
	TrackPopupMenuEx(m_SEMenu,flag,x,y,m_hWnd,lptpm);
}


void CIEPlus::DisplayImage(BOOL bDisplay)
{
	HKEY key;
	DWORD disp;
	TCHAR* szVal = _T("Display Inline Images");
	TCHAR szBuf[20];
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_IE_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return;
	if(bDisplay)
		_tcscpy(szBuf,_T("yes"));
	else
		_tcscpy(szBuf,_T("no"));
	RegSetValueEx(key,szVal,NULL,REG_SZ,(BYTE*)szBuf,_tcslen(szBuf));
	RegFlushKey(key);
	RegCloseKey(key);
	::SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) GD_IE_REG,
			SMTO_ABORTIFHUNG | SMTO_BLOCK, 1 * 1000, &disp ); // wait 3 seconds on each window}
}

void CIEPlus::DisplayVideo(BOOL bDisplay)
{
	HKEY key;
	DWORD disp;
	TCHAR* szVal = _T("Display Inline Videos");
	TCHAR szBuf[20];
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_IE_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return;
	if(bDisplay)
		_tcscpy(szBuf,_T("yes"));
	else
		_tcscpy(szBuf,_T("no"));
	RegSetValueEx(key,szVal,NULL,REG_SZ,(BYTE*)szBuf,_tcslen(szBuf));
	RegFlushKey(key);
	RegCloseKey(key);
	::SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) GD_IE_REG,
			SMTO_ABORTIFHUNG | SMTO_BLOCK, 1 * 1000, &disp ); // wait 3 seconds on each window}
}

void CIEPlus::DisplayAnim(BOOL bDisplay)
{
	HKEY key;
	DWORD disp;
	TCHAR* szVal = _T("Play_Animations");
	TCHAR szBuf[20];
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_IE_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return;
	if(bDisplay)
		_tcscpy(szBuf,_T("yes"));
	else
		_tcscpy(szBuf,_T("no"));
	RegSetValueEx(key,szVal,NULL,REG_SZ,(BYTE*)szBuf,_tcslen(szBuf));
	RegFlushKey(key);
	RegCloseKey(key);
	::SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) GD_IE_REG,
			SMTO_ABORTIFHUNG | SMTO_BLOCK, 1 * 1000, &disp ); // wait 3 seconds on each window}
}

void CIEPlus::PlaySound(BOOL bPlay)
{
	HKEY key;
	DWORD disp;
	TCHAR* szVal = _T("Play_Background_Sounds");
	TCHAR szBuf[20];
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_IE_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return;
	if(bPlay)
		_tcscpy(szBuf,_T("yes"));
	else
		_tcscpy(szBuf,_T("no"));
	RegSetValueEx(key,szVal,NULL,REG_SZ,(BYTE*)szBuf,_tcslen(szBuf));
	RegFlushKey(key);
	RegCloseKey(key);
	::SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) GD_IE_REG,
			SMTO_ABORTIFHUNG | SMTO_BLOCK, 1 * 1000, &disp ); // wait 3 seconds on each window}
}

DWORD WINAPI RasListener( LPVOID lpParam ) 
{ 
	CProxyManager* pm=reinterpret_cast<CProxyManager*>(lpParam);
	HANDLE hRasEvent=CreateEvent(NULL,FALSE,FALSE,_T("RAS Listener"));
	if(!hRasEvent)
	{
		ReportErrMsg("In CIEPlus::RasListener");
		return 1;
	}
	while(1)
	{
		if(RasConnectionNotification((HRASCONN)INVALID_HANDLE_VALUE,hRasEvent,RASCN_Connection|RASCN_Disconnection))
		{
			ReportErrMsg("In CIEPlus::RasListener");
			CloseHandle(hRasEvent);
			return 1;
		}
		if(WAIT_OBJECT_0==WaitForSingleObject(hRasEvent,INFINITE))
		{
			pm->SetReqFresh();
		}
	}
    return 0; 
} 


BOOL CIEPlus::SetupRasListener()
{
	DWORD dwThreadId;
	DWORD dwThrdParam = (DWORD)(this->m_pPM); 

    HANDLE hThread = CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        RasListener,                  // thread function 
        (LPVOID)dwThrdParam,                // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
 
   if (hThread == NULL) 
   {
      ReportErrMsg("In CIEPlus::SetupRasListener CreateThread failed."); 
	  return FALSE;
   }
  	if(hThread!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
	}
	return TRUE;
}