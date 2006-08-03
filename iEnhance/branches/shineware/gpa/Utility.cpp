#include <stdafx.h>
#include "Utility.h"

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