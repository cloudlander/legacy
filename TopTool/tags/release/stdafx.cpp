// stdafx.cpp : 只包括标准包含文件的源文件
// TopTool.pch 将是预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

vector<WndElem> g_WndVec;

BOOL CALLBACK EnumProc(HWND hwnd,LPARAM lParam)
{
	TCHAR buf[255];
	LONG style;
	if(::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
	{
		if(::GetWindowText(hwnd,buf,255) > 0 && buf[0] != _T('\0') && 0 != _tcscmp(buf,_T("Program Manager")))
		{
			style = ::GetWindowLong(hwnd,GWL_EXSTYLE);
			if(WS_EX_TOPMOST & style)
				g_WndVec.push_back(WndElem(hwnd,CString(buf),TRUE));
			else
				g_WndVec.push_back(WndElem(hwnd,CString(buf),FALSE));
		}
	}
	return TRUE;
}
