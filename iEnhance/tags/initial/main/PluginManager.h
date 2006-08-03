// PluginManager.h: interface for the CPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMANAGER_H__255C9240_9A79_420B_9768_403276C81368__INCLUDED_)
#define AFX_PLUGINMANAGER_H__255C9240_9A79_420B_9768_403276C81368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL (*MAINPROC)(HWND,HWND,void*,int sess);	// other interfaces appending

typedef void (*BEFORENAVIGATE2)(IDispatch*,VARIANT*,VARIANT*,
									VARIANT*,VARIANT*,VARIANT*,VARIANT_BOOL*);
typedef void (*COMMANDSTATECHANGE)(long,VARIANT_BOOL);
typedef void (*DOCUMENTCOMPLETE)(IDispatch*,VARIANT*);
typedef void (*DOWNLOADBEGIN)();
typedef void (*DOWNLOADCOMPLETE)();
typedef void (*NAVIGATECOMPLETE2)(IDispatch*,VARIANT*);
typedef void (*NEWWINDOW2)(IDispatch**,VARIANT_BOOL*);
typedef void (*FULLSCREEN)(VARIANT_BOOL);
typedef void (*MENUBAR)(VARIANT_BOOL);
typedef void (*QUIT)();
typedef void (*STATUSBAR)(VARIANT_BOOL);
typedef void (*THEATERMODE)(VARIANT_BOOL);
typedef void (*TOOLBAR)(VARIANT_BOOL);
typedef void (*VISIBLE)(VARIANT_BOOL);
typedef void (*PROGRESSCHANGE)(long,long);
typedef void (*PROPERTYCHANGE)(BSTR);
typedef void (*STATUSTEXTCHANGE)(BSTR);
typedef void (*TITLECHANGE)(BSTR);	

class Plugin
{
public:
	BOOL bActive;				// is enabled?
	HMODULE hDll;
	MAINPROC _proc_main;		// others appending
	
	BEFORENAVIGATE2			_proc_beforenavigate2;
	COMMANDSTATECHANGE		_proc_commandstatechange;
	DOCUMENTCOMPLETE		_proc_documentcomplete;
	DOWNLOADBEGIN			_proc_downloadbegin;
	DOWNLOADCOMPLETE		_proc_downloadcomplete;
	NAVIGATECOMPLETE2		_proc_navigatecomplete2;
	NEWWINDOW2				_proc_newwindow2;
	FULLSCREEN				_proc_fullscreen;
	MENUBAR					_proc_menubar;
	QUIT					_proc_quit;
	STATUSBAR				_proc_statusbar;
	THEATERMODE				_proc_theatermode;
	TOOLBAR					_proc_toolbar;
	VISIBLE					_proc_visible;
	PROGRESSCHANGE			_proc_progresschange;
	PROPERTYCHANGE			_proc_propertychange;
	STATUSTEXTCHANGE		_proc_statustextchange;
	TITLECHANGE				_proc_titlechange;
	
	Plugin(HMODULE hdll,MAINPROC _main)
	{
		bActive=FALSE;
		hDll=hdll;
		_proc_main=_main;

		_proc_beforenavigate2=NULL;
		_proc_commandstatechange=NULL;
		_proc_documentcomplete=NULL;
		_proc_downloadbegin=NULL;
		_proc_downloadcomplete=NULL;
		_proc_navigatecomplete2=NULL;
		_proc_newwindow2=NULL;
		_proc_fullscreen=NULL;
		_proc_menubar=NULL;
		_proc_quit=NULL;
		_proc_statusbar=NULL;
		_proc_theatermode=NULL;
		_proc_toolbar=NULL;
		_proc_visible=NULL;
		_proc_progresschange=NULL;
		_proc_propertychange=NULL;
		_proc_statustextchange=NULL;
		_proc_titlechange=NULL;
	}
};

#define IDM_PLUGIN_BASE		402

class CPluginManager  
{
public:	// for debug
	vector<Plugin> vecPlugin;			// vector of Plugins
	vector<int>	vecSession;				// vector of all active Plugins' session
	HMENU hmenu;
public:
	CPluginManager();
	virtual ~CPluginManager();

	int GetSize()
	{
		return vecPlugin.size();
	}


	BOOL Init(HMENU menu);		// load dlls and append the iEnhance's menu
	MAINPROC Execute(int id);		// execute dll's main procedure
	void Call(DISPID msg,DISPPARAMS* pDispParams);			// @not implemented, called when IE's event invoked
	void CloseSession(int sess);
	void EnumSessionBEFORENAVIGATE2( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionCOMMANDSTATECHANGE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionDOCUMENTCOMPLETE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionDOWNLOADBEGIN( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionDOWNLOADCOMPLETE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionNAVIGATECOMPLETE2( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionNEWWINDOW2( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionFULLSCREEN( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionMENUBAR( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionQUIT( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionSTATUSBAR( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionTHEATERMODE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionTOOLBAR( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionVISIBLE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionPROGRESSCHANGE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionPROPERTYCHANGE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionSTATUSTEXTCHANGE( char* FuncName,DISPPARAMS* pDispParams);
	void EnumSessionTITLECHANGE( char* FuncName,DISPPARAMS* pDispParams);
	void SetSessionActive(int id,BOOL bActive);

	static BOOL GetDllDescription(const TCHAR* szFileName,TCHAR* Desc);
};

#endif // !defined(AFX_PLUGINMANAGER_H__255C9240_9A79_420B_9768_403276C81368__INCLUDED_)
