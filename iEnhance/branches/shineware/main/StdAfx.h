// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__B5FC8C60_0056_4D8F_9B59_09B2525EA515__INCLUDED_)
#define AFX_STDAFX_H__B5FC8C60_0056_4D8F_9B59_09B2525EA515__INCLUDED_
#define ISOLATION_AWARE_ENABLED 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#define _ATL_APARTMENT_THREADED

#ifndef _WIN32_IE
#define _WIN32_IE	0x0500
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <tchar.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#define g_hInst	_Module.m_hInst
#define MAX_TEXT_LENGTH		255
#define MIN_COMBO_SIZE		150
#define MAX_COMBO_SIZE		500
#include <atlcom.h>

#include <atlwin.h>
#include <atlhost.h>

#include <CommCtrl.h>
#include <wininet.h>

#include "Utility.h"

#include <string>
#include <vector>
#include <map>
#include <stack>

using namespace std;

typedef basic_string<TCHAR,char_traits<TCHAR>, allocator<TCHAR> > tstring;

#import "d:\winnt\system32\Macromed\Flash\Flash.ocx" exclude("IServiceProvider") no_namespace rename("DISPPARAMS","tagDISPPARAMS") rename("EXCEPINFO","tagEXCEPINFO") 

// 全局配置数据
class GlobalData
{
public:
	tstring IEReg;
	tstring SystemReg;
	tstring ProxyReg;		// 程序注册位置
	tstring MainReg;
	tstring TypedReg;
	char ModulePath[260];		// 程序所在路径，从Reg里读取
	int xCenter;			// 显示器中心
	int yCenter;

	GlobalData()
	{
		IEReg = _T("Software\\Microsoft\\Internet Explorer\\Main");
		SystemReg = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings");
		ProxyReg = _T("Software\\iEnhance\\ProxyServer");
		MainReg = _T("Software\\iEnhance");
		TypedReg = _T("Software\\iEnhance\\Typed");
//*	
		HKEY key;
		DWORD disp;
		DWORD type = REG_SZ;
		TCHAR *szVal = _T("Module");
		TCHAR szBuf[260];
		DWORD size = 260;
		if(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\iEnhance"),
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
		{
			if(REG_CREATED_NEW_KEY == disp)		// 首次创建,不该发生!
			{
				ATLASSERT(FALSE);
				RegCloseKey(key);
			}

			if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
			{
				int i = strlen(szBuf);
				if('\\' != szBuf[i-1])
				{
					szBuf[i] = '\\';
					szBuf[i+1] = '\0';
				}
				strcpy(ModulePath,szBuf);
			}
			RegCloseKey(key);
		}
//*/
		xCenter =::GetSystemMetrics(SM_CXSCREEN)/2;
		yCenter =::GetSystemMetrics(SM_CYSCREEN)/2;
	}
};

static GlobalData g_GD;

#define	GD_SYSTEM_PROXY_REG		_T(g_GD.SystemReg.c_str())
#define GD_PROXY_REG			_T(g_GD.ProxyReg.c_str())
#define GD_MAIN_REG				_T(g_GD.MainReg.c_str())
#define GD_TYPED_REG			_T(g_GD.TypedReg.c_str())
#define GD_X_CENTER				g_GD.xCenter
#define GD_Y_CENTER				g_GD.yCenter
#define GD_MODULE_PATH			g_GD.ModulePath
#define GD_IE_REG				_T(g_GD.IEReg.c_str())
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B5FC8C60_0056_4D8F_9B59_09B2525EA515__INCLUDED)
