// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5CAF0DD7_4024_455E_918C_CAECCE6D7217__INCLUDED_)
#define AFX_STDAFX_H__5CAF0DD7_4024_455E_918C_CAECCE6D7217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define ISOLATION_AWARE_ENABLED 1
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <tchar.h>
#pragma warning(disable:4786)
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
#include <wininet.h>

typedef basic_string<TCHAR,char_traits<TCHAR>, allocator<TCHAR> > tstring;
#pragma warning(disable : 4146)	//see Q231931 for explaintation
#import <mshtml.tlb> no_auto_exclude
extern	int xCenter;
extern	int yCenter;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5CAF0DD7_4024_455E_918C_CAECCE6D7217__INCLUDED_)
