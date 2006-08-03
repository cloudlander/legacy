#ifndef SHUTILSH
#define SHUTILSH

/////////////////////////////////////////////////////////////
// shell extension name
#define	SHELLEXNAME				_T("EasyMD5")

#ifdef CTXMENU_ALL_CODE
/////////////////////////////////////////////////////////////
// safe data access for data shared between threads
extern CRITICAL_SECTION g_critSectionBreak;
template <class T> T GET_SAFE( T a ) {EnterCriticalSection(&g_critSectionBreak); T z = a; LeaveCriticalSection(&g_critSectionBreak); return z;}
template <class T> void SET_SAFE( T & a , T b) {EnterCriticalSection(&g_critSectionBreak); a = b; LeaveCriticalSection(&g_critSectionBreak);}

/////////////////////////////////////////////////////////////
// just like TRACE0
#ifdef _DEBUG
#define ODS(sz) {if (strlen(sz)>0) {OutputDebugString(SHELLEXNAME); OutputDebugString("=>");} OutputDebugString(sz);}
#else
#define ODS(x)
#endif

#else
#define ODS(sz)
#endif

#define ResultFromShort(i)  ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(i)))

/////////////////////////////////////////////////////////////
// macro to determine # of args in an array
#define ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))

/////////////////////////////////////////////////////////////
// pump msgs
extern void	ShMsgPump();

/////////////////////////////////////////////////////////////
// conversion routines
extern int  LocalToWideChar(LPWSTR pWide, LPTSTR pLocal, DWORD dwChars);
extern int  WideCharToLocal(LPTSTR pLocal, LPWSTR pWide, DWORD dwChars);

extern STDMETHODIMP CreateShellExtMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags, HBITMAP hbmp);
extern STDMETHODIMP CreateShellExtMenuEx(LPVOID param,HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags, HBITMAP hbmp);
extern STDMETHODIMP GetSelectedFiles(LPCITEMIDLIST pIDFolder, LPDATAOBJECT & pDataObj, CStringArray &csaPaths);


LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);

#endif