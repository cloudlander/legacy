#include <stdafx.h>
#include <ras.h>
#include <raserror.h>
#include "Utility.h"
#pragma comment(lib,"rasapi32.lib")

void ReportErrMsg()
{
	DWORD err = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );	
}

void ReportErrMsg(int arg)
{
	LPVOID MsgBuf;
	TCHAR argBuf[100];
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &MsgBuf,
		0,
		NULL 
	);
	_stprintf(argBuf,_T("argument = %d"),arg);
	_tcscat((LPTSTR)MsgBuf,argBuf);
	MessageBox( NULL, (LPTSTR)MsgBuf, "Error", MB_OK | MB_ICONINFORMATION );	
}

void ReportErrMsg(TCHAR* msg)
{
	LPVOID MsgBuf;
	TCHAR argBuf[100];
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &MsgBuf,
		0,
		NULL 
	);
	_stprintf(argBuf,_T("msg = %s"),msg);
	_tcscat((LPTSTR)MsgBuf,argBuf);
	MessageBox( NULL, (LPTSTR)MsgBuf, "Error", MB_OK | MB_ICONINFORMATION );	
}

BOOL GetActiveRasInterface(TCHAR* buf,int size)
{
	DWORD dwCb = sizeof(RASCONN);
	DWORD dwErr = ERROR_SUCCESS;
	DWORD dwRetries = 5;
	DWORD dwConnections = 0;
	RASCONN* lpRasConn = NULL;

	//
	// Loop through in case the information from RAS changes between calls.
	//
	while (dwRetries--)
	{
		//
		// If the memory is allocated, free it.
		//
		if (NULL != lpRasConn)
		{
			HeapFree(GetProcessHeap(), 0, lpRasConn);
			lpRasConn = NULL;
		}
		//
		// Allocate the size needed for the RAS structure.
		//
		lpRasConn = (RASCONN*)HeapAlloc(GetProcessHeap(), 0, dwCb);
		if (NULL == lpRasConn)
		{
			dwErr = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		//
		// Set the structure size for version checking purposes.
		//
		lpRasConn->dwSize = sizeof(RASCONN);
		//
		// Call the RAS API then exit the loop if we are successful or an unknown
		// error occurs.
		//
		dwErr = RasEnumConnections(
					lpRasConn,
					&dwCb,
					&dwConnections);
		if (ERROR_BUFFER_TOO_SMALL != dwErr)
		{
			break;
		}
	}
	//
	// In the success case, print the names of the connections.
	//
	if (ERROR_SUCCESS == dwErr)
	{
#if 0
		DWORD i;

		printf("The following RAS connections are currently active\n\n");
		for (i = 0; i < dwConnections; i++)
		{
			printf("%s\n", lpRasConn[i].szEntryName);
		}
#endif
		// only return the firstly found interface name
		if(dwConnections<1)
		{
			buf[0]=_T('\0');
		}
		else 
		{
			if(size+1<_tcslen(lpRasConn[0].szEntryName))
			{
				ReportErrMsg(_T("In GetActiveRasInterface: not enough buffer"));
				dwErr=ERROR_BUFFER_TOO_SMALL;
				buf[0]=_T('\0');
			}
			else
				_tcscpy(buf,lpRasConn[0].szEntryName);		
		}
	}
	else
	{
//		printf("RasEnumConnections failed: Error = %d\n", dwErr);
	}
	//
	// Free the memory if necessary.
	//
	if (NULL != lpRasConn)
	{
		HeapFree(GetProcessHeap(), 0, lpRasConn);
		lpRasConn = NULL;
	}
	return dwErr;
}