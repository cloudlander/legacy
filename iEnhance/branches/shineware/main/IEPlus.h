// IEPlus.h : Declaration of the CIEPlus

#ifndef __IEPLUS_H_
#define __IEPLUS_H_

#include <windowsx.h>
#include <shlguid.h>
#include <mshtmhst.h>
#include <mshtmdid.h>
#include "resource.h"       // main symbols
#include "SearchEngine.h"
#include "ProxyManager.h"
#include "PluginManager.h"
#include "MoreWnd.h"
#include "QuickSavedlg.h"
#include "OptionDlg.h"

#define TB_CLASS_NAME   "ToolBar"
#define TB_MIN_SIZE_X   10
#define TB_MIN_SIZE_Y   23

#define TB_MAX_SIZE_Y   23

#define IMG_WIDTH		22
#define IMG_HEIGHT		20

#define MAX_MENU_ENTRY	15

/////////////////////////////////////////////////////////////////////////////
// CIEPlus
class ATL_NO_VTABLE CIEPlus : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIEPlus, &CLSID_IEPlus>,
	public IDeskBand, 
    public IInputObject, 
    public IObjectWithSite,
    public IPersistStream,
	public IDispatch
{
public:
	CIEPlus();
	~CIEPlus();

DECLARE_REGISTRY_RESOURCEID(IDR_IEPLUS)
DECLARE_NOT_AGGREGATABLE(CIEPlus)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIEPlus)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY(IDockingWindow)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IDeskBand)
	COM_INTERFACE_ENTRY(IInputObject)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IIEPlus
public:
   //IOleWindow methods
   STDMETHOD (GetWindow) (HWND*);
   STDMETHOD (ContextSensitiveHelp) (BOOL);

   //IDockingWindow methods
   STDMETHOD (ShowDW) (BOOL fShow);
   STDMETHOD (CloseDW) (DWORD dwReserved);
   STDMETHOD (ResizeBorderDW) (LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);

   //IDeskBand methods
   STDMETHOD (GetBandInfo) (DWORD, DWORD, DESKBANDINFO*);

   //IInputObject methods
   STDMETHOD (UIActivateIO) (BOOL, LPMSG);
   STDMETHOD (HasFocusIO) (void);
   STDMETHOD (TranslateAcceleratorIO) (LPMSG);

   //IObjectWithSite methods
   STDMETHOD (SetSite) (IUnknown*);
   STDMETHOD (GetSite) (REFIID, LPVOID*);

   //IPersistStream methods
   STDMETHOD (GetClassID) (LPCLSID);
   STDMETHOD (IsDirty) (void);
   STDMETHOD (Load) (LPSTREAM);
   STDMETHOD (Save) (LPSTREAM, BOOL);
   STDMETHOD (GetSizeMax) (ULARGE_INTEGER*);

   // IDispatch Methods
   STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
   STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
   STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
                     VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   
public:
	int m_nSel;

private:
	IInputObjectSite *m_pSite;
	CComPtr<IWebBrowser2> m_pFrameWB;
	IDocHostUIHandler* m_pDocUI;

	DWORD m_dwWBCookie;
	DWORD m_dwViewMode;
	DWORD m_dwBandID;
	BOOL m_bFocus;

	HWND m_hWebBrower;
	HWND m_hwndParent;
	HWND m_hWnd;
	HWND m_hwndToolbar;
	HWND m_hCombo;
	HWND m_hedit;
	int m_nComboOrd;
	int m_nComboSize;
	
	int  m_nClosed;
	BOOL m_bAutoClose;
	BOOL m_bBeep;

	HMENU m_PopupMenu;
	HMENU m_MainMenu;
	HMENU m_ProxyMenu;
	HMENU m_HttpMenu;	
	HMENU m_SEMenu;
	HIMAGELIST himg,himgcold;
	CMoreWnd* m_pMoreWnd;
	DWORD orgProc,orgEditProc,orgProc2;
	vector<ProxyEntry> m_HttpList;

public:
	CProxyManager*	m_pPM;
	CSearchEngine*  m_pSE;
	CPluginManager* m_pPlugMgr;
private:
	void PlaySound(BOOL bPlay=TRUE);
	void DisplayAnim(BOOL bDisplay=TRUE);
	void DisplayVideo(BOOL bDisplay=TRUE);
	void DisplayImage(BOOL bDisplay=TRUE);
	BOOL OnDestroy();
	BOOL ShowMoreWnd();
	void ShowProxyPopup(UINT flag,int x,int y,LPTPMPARAMS lptpm);
	void Store();
	void Init();
	BOOL SetupRasListener();
	void FocusChange(BOOL);
	LRESULT OnKillFocus(void);
	LRESULT OnSetFocus(void);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc2(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate();
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(LPARAM);
	BOOL RegisterAndCreateWindow(void);
	BOOL CreateToolBar();
	
	void AdviseEvent();
	void UnadviseEvent();
	HRESULT GetConnectionPoint(LPUNKNOWN pUnk, REFIID riid, LPCONNECTIONPOINT* ppCP);
	stack<DWORD> _stackAnchorCookies;
	enum AdviseType
	{
		Advise,
			Unadvise
	};
	void ManageAnchorsEventSink(AdviseType adviseType);
	
	static BOOL IsWinXP();
public:
	void ShowSePopup(UINT flag,int x,int y,LPTPMPARAMS lptpm);
	vector<ProxyEntry>& GetProxyList(int type = 0);
	void RefreshProxyMenu();
	
};

#endif //__IEPLUS_H_
