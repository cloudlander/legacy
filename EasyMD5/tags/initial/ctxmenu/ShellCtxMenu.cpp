#include "stdafx.h"
#include "resource.h"
#include "priv.h"
#include "ShellExt.h"
#include "CtxMenu.h"

#include <shlobj.h>
#include <shlguid.h>
#include "io.h"

// utilities
#include "ShUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
void
DisplayErrorText()
{
	DWORD dwLastError=GetLastError();
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;
	
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;
	
    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //
	
	//
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //
	
    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
		AfxMessageBox(MessageBuffer);
		/*
        //
        // Output message string on stderr.
        //
        DWORD dwBytesWritten;
        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            MessageBuffer,
            dwBufferLength,
            &dwBytesWritten,
            NULL
            );
*/		
        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }
	
    //
    // If we loaded a message source, unload it.
    //
}
/*----------------------------------------------------------------

  FUNCTION: CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO)

  PURPOSE: Called by the shell after the user has selected on of the
           menu items that was added in QueryContextMenu(). This is 
           the function where you get to do your real work!!!

  PARAMETERS:
    lpcmi - Pointer to an CMINVOKECOMMANDINFO structure

  RETURN VALUE:


  COMMENTS:
 
----------------------------------------------------------------*/

STDMETHODIMP CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
	// look at all the MFC stuff in here! call this to allow us to 
	// not blow up when we try to use it.
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	HINSTANCE hInst = AfxGetInstanceHandle();

    ODS("CShellExt::InvokeCommand()\r\n");

	if(m_exePath==_T(""))
		return NOERROR;

	HWND hParentWnd = lpcmi->hwnd;
                                      
	HRESULT hr = NOERROR;

    //If HIWORD(lpcmi->lpVerb) then we have been called programmatically
    //and lpVerb is a command that should be invoked.  Otherwise, the shell
    //has called us, and LOWORD(lpcmi->lpVerb) is the menu ID the user has
    //selected.  Actually, it's (menu ID - idCmdFirst) from QueryContextMenu().
	UINT idCmd = 0;

   if (!HIWORD(lpcmi->lpVerb)) 
   {
      idCmd = LOWORD(lpcmi->lpVerb);

	  CString buf;
		// process it
		switch (idCmd) 
      {
		case 0: // Create MD5
			{
				/* create a temp file */
				TCHAR szTempName[260];
				TCHAR szTempPath[260];
				GetTempPath(260,szTempPath);
				GetTempFileName(szTempPath, // directory for temp files 
					_T("MD5"),                // temp file name prefix 
					0,                    // create unique name 
					szTempName);          // buffer for name 
				
				CStdioFile of;
				CFileException ex;
				if(FALSE==of.Open(szTempName,CFile::modeWrite,&ex))
				{
					TCHAR* err=new TCHAR[1024];
					ex.GetErrorMessage(err,1024);
					AfxMessageBox(CString(_T("NEW TEMP FILE"))+err);
				}
				else
				{
					int iFiles = m_csaPaths.GetSize();
					for (int j=0; j < iFiles; j++) 
					{
						// get the current file. this array is filled in CShellExt::Initialize
						buf = m_csaPaths.GetAt(j);
						of.WriteString(buf+_T("\n"));
					}
					of.Close();
				}
			
				buf=m_exePath+CString(_T(" @\""))+szTempName+CString(_T("\""));
//				AfxMessageBox(buf);
				
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				
				ZeroMemory( &si, sizeof(si) );
				si.cb = sizeof(si);
				ZeroMemory( &pi, sizeof(pi) );
				
				// Start the child process. 
				if( !CreateProcess( NULL, // No module name (use command line). 
				    (TCHAR*)(LPCTSTR)buf, // Command line. 
					NULL,             // Process handle not inheritable. 
					NULL,             // Thread handle not inheritable. 
					FALSE,            // Set handle inheritance to FALSE. 
					0,                // No creation flags. 
					NULL,             // Use parent's environment block. 
					NULL,             // Use parent's starting directory. 
					&si,              // Pointer to STARTUPINFO structure.
					&pi )             // Pointer to PROCESS_INFORMATION structure.
					) 
				{
					DisplayErrorText();
				}
			}
			break;
		case 1:		// check md5
			{
				buf=m_exePath+CString(_T(" \""))+m_csaPaths.GetAt(0)+CString(_T("\""));
//				AfxMessageBox(buf);
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				
				ZeroMemory( &si, sizeof(si) );
				si.cb = sizeof(si);
				ZeroMemory( &pi, sizeof(pi) );
				
				// Start the child process. 
				if( !CreateProcess( NULL, // No module name (use command line). 
					(TCHAR*)(LPCTSTR)buf, // Command line. 
					NULL,             // Process handle not inheritable. 
					NULL,             // Thread handle not inheritable. 
					FALSE,            // Set handle inheritance to FALSE. 
					0,                // No creation flags. 
					NULL,             // Use parent's environment block. 
					NULL,             // Use parent's starting directory. 
					&si,              // Pointer to STARTUPINFO structure.
					&pi )             // Pointer to PROCESS_INFORMATION structure.
					) 
				{
					DisplayErrorText();
				}
			}
			break;
		default:
			break;
		}	// switch on command
	}

   return hr;
}


////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: CShellExt::GetCommandString(...)
//
//  PURPOSE: Retrieve various text strinsg associated with the context menu
//
//	Param			Type			Use
//	-----			----			---
//	idCmd			UINT			ID of the command
//	uFlags			UINT			which type of info are we requesting
//	reserved		UINT *			must be NULL
//	pszName			LPSTR			output buffer
//	cchMax			UINT			max chars to copy to pszName
//
////////////////////////////////////////////////////////////////////////

STDMETHODIMP CShellExt::GetCommandString(UINT idCmd,
                                         UINT uFlags,
                                         UINT FAR *reserved,
                                         LPSTR pszName,
                                         UINT cchMax)
{
   ODS("CShellExt::GetCommandString()\r\n");
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	HINSTANCE hInst = AfxGetInstanceHandle();

	switch (uFlags) 
   {
	case GCS_HELPTEXT:		// fetch help text for display at the bottom of the 
							// explorer window
		switch (idCmd)
		{
			case 0:
            strncpy(pszName, "Get the md5 digests of the selected file(s)", cchMax);
				break;
			case 1:
            strncpy(pszName, "Check the md5 digests with the selected file(s)", cchMax);
				break;
         default:
            strncpy(pszName, SHELLEXNAME, cchMax);
				break;
		}
		break;

	case GCS_VALIDATE:
		break;

	case GCS_VERB:			// language-independent command name for the menu item 
		switch (idCmd)
		{
			case 0:
            strncpy(pszName, "Operation1", cchMax);
				break;
			case 1:
            strncpy(pszName, "Operation2", cchMax);
				break;
         default:
            strncpy(pszName, SHELLEXNAME, cchMax);
				break;
		}

		break;
	}
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: CShellExt::QueryContextMenu(HMENU, UINT, UINT, UINT, UINT)
//
//  PURPOSE: Called by the shell just before the context menu is displayed.
//           This is where you add your specific menu items.
//
//  PARAMETERS:
//    hMenu      - Handle to the context menu
//    indexMenu  - Index of where to begin inserting menu items
//    idCmdFirst - Lowest value for new menu ID's
//    idCmtLast  - Highest value for new menu ID's
//    uFlags     - Specifies the context of the menu event
//
//  RETURN VALUE:
//
//
//  COMMENTS:
//
///////////////////////////////////////////////////////////////////////////

STDMETHODIMP CShellExt::QueryContextMenu(HMENU hMenu,
                                         UINT indexMenu,
                                         UINT idCmdFirst,
                                         UINT idCmdLast,
                                         UINT uFlags)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

   return CreateShellExtMenuEx(this,hMenu, 
                              indexMenu, 
                              idCmdFirst, 
                              idCmdLast, 
                              uFlags, 
                              (HBITMAP)m_menuBmp.GetSafeHandle());
}






















