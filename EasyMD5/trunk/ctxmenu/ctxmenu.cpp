// ctxmenu.cpp : Defines the initialization routines for the DLL.
//

#pragma data_seg(".text")
#include "stdafx.h"
#define INITGUID
#include <initguid.h>
#include <shlguid.h>
#include "ShellExt.h"
#include "shutils.h"
#include "ctxmenu.h"
#pragma data_seg()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned int  g_cRefThisDll = 0;


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCtxmenuApp

BEGIN_MESSAGE_MAP(CCtxmenuApp, CWinApp)
	//{{AFX_MSG_MAP(CCtxmenuApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtxmenuApp construction

CCtxmenuApp::CCtxmenuApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCtxmenuApp object

CCtxmenuApp theApp;

BOOL CCtxmenuApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef CTXMENU_ALL_CODE
	InitializeCriticalSection(&g_critSectionBreak);
#endif

	INITCOMMONCONTROLSEX cct;
	cct.dwSize = sizeof(INITCOMMONCONTROLSEX);
	cct.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
	if(FALSE == InitCommonControlsEx(&cct))
		return FALSE;
		
	return CWinApp::InitInstance();
}

int CCtxmenuApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef CTXMENU_ALL_CODE
	DeleteCriticalSection(&g_critSectionBreak);
#endif
	
	return CWinApp::ExitInstance();
}

///////////////////////////////////////////////////////////////////////////
//
//  STDAPI DllCanUnloadNow(void)
//
//  PURPOSE: Tell whoever's interested if they can unload us or not.
//
//  Param				Type			Use
//	-----				----			---
//	none
//
//  Returns 
//		S_OK if it's OK to unload us.
//
///////////////////////////////////////////////////////////////////////////

STDAPI DllCanUnloadNow(void)
{
    ODS("In DLLCanUnloadNow\r\n");
	
    return (g_cRefThisDll == 0 ? S_OK : S_FALSE);
}

///////////////////////////////////////////////////////////////////////////
//
//  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
//
//  PURPOSE: Create a class object of the type requested. This is how
//	Explorer talks to us.
//
//  Param			Type			Use
//	-----			----			---
//	rclsif			REFCLSID		class ID reference
//	riid			REFIID			interface ID reference
//	ppvOut			LPVOID *		ptr to an instance of the requested interface
//
//  Returns 
//		STDAPI status code
//
///////////////////////////////////////////////////////////////////////////
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{
    ODS("In DllGetClassObject\r\n");
	
    *ppvOut = NULL;
	
    // file
    if (IsEqualIID(rclsid, CLSID_EASYMD5))
    {
        CShellExtClassFactory *pcf;
		try {
			pcf = new CShellExtClassFactory;
		} catch (CMemoryException *e) {
			e->Delete();
			pcf=NULL;
		}
		
        return pcf->QueryInterface(riid, ppvOut);
    }
	
    return CLASS_E_CLASSNOTAVAILABLE;
}