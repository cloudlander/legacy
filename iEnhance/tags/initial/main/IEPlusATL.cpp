// IEPlusATL.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f IEPlusATLps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "IEPlusATL.h"

#include "IEPlusATL_i.c"
#include "IEPlus.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_IEPlus, CIEPlus)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, NULL);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	int i;
	int size = 260;
	TCHAR buf[260];
	GetModuleFileName(_Module.m_hInst,buf,260);
	i = _tcslen(buf);
	while(i>=0)
	{
		if(_T('\\') == buf[i])
			break;
		i--;
	}
	buf[i] = _T('\0');

	HKEY key;
	DWORD disp;
	DWORD type = REG_SZ;
	TCHAR *szVal = _T("Module");
	size = i;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\iEnhance"),
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return E_FAIL;
	if(ERROR_SUCCESS != RegSetValueEx(key,szVal,NULL,type,(BYTE*)buf,size))
		return E_FAIL;
	RegFlushKey(key);
	RegCloseKey(key);
    // registers object, typelib and all interfaces in typelib
//    return _Module.RegisterServer(TRUE);
	return _Module.RegisterServer();
//	tstring tmp=buf;
//	tmp+=_T("\\html\\first.htm");
//	ShellExecute(HWND_DESKTOP,NULL,tmp.c_str(),NULL,buf,SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	// 此处不能直接Delete! 怪事
/*
	if(ERROR_SUCCESS != RegDeleteValue(HKEY_LOCAL_MACHINE,
		_T("Software\\Microsoft\\Internet Explorer\\Toolbar\\{83C64D70-F259-4384-8443-217351436BE6}")))
		return E_FAIL;
*/
	// 另外NT/2000平台上并不递规删除字键，所以此项工作右uninstaller完成
	HKEY key;
	DWORD disp;
	DWORD type = REG_SZ;
	TCHAR *szVal = _T("{83C64D70-F259-4384-8443-217351436BE6}");
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Internet Explorer\\Toolbar"),
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return E_FAIL;
	if(ERROR_SUCCESS != RegDeleteValue(key,szVal))
	{
		RegCloseKey(key);
		return E_FAIL;
	}
	RegFlushKey(key);
	RegCloseKey(key);
//    return _Module.UnregisterServer(TRUE);
	return _Module.UnregisterServer(FALSE);
}


