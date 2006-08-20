// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__14037B98_0B4F_4A97_A44B_6207948F08B8__INCLUDED_)
#define AFX_STDAFX_H__14037B98_0B4F_4A97_A44B_6207948F08B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxhtml.h>			// MFC HTML view support
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <list>

using namespace std;
#include <math.h>
//#include "CJLibrary.h"
#include "Matrix.h"
typedef CMatrixT<float> CMatrix;
#define _CJXLIB_INLINE inline
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#include <..\src\afximpl.h>
#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif

#define IDC_BUTTON_HIDE 5012
#define IDC_BUTTON_MINI 5013
#define _delete(p){if(p){delete p;p=NULL;}}
#define _deleteMeta(p){if(p){::DeleteEnhMetaFile(p);p=NULL;}}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__14037B98_0B4F_4A97_A44B_6207948F08B8__INCLUDED_)
