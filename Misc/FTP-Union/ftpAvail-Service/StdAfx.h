// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__49010353_0E6D_4681_ABDC_8DE9C30C5201__INCLUDED_)
#define AFX_STDAFX_H__49010353_0E6D_4681_ABDC_8DE9C30C5201__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxinet.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

// TODO: reference additional headers your program requires here
#import "c:\Program Files\Common Files\System\ado\msado15.dll"\
    no_namespace rename("EOF", "EndOfFile")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__49010353_0E6D_4681_ABDC_8DE9C30C5201__INCLUDED_)
