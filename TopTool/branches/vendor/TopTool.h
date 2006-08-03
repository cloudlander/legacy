// TopTool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
extern vector<WndElem> g_WndVec;
extern BOOL CALLBACK EnumProc(HWND hwnd,LPARAM lParam);

#include "resource.h"		// 主符号
#include "SystemTray.h"

// CTopToolApp:
// 有关此类的实现，请参阅 TopTool.cpp
//

class CTopToolApp : public CWinApp
{
public:
	CSystemTray m_SysTrayWnd;
	CFrameWnd*	m_pTempWnd;
public:
	CTopToolApp();
	
// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAbout();
	afx_msg void OnSelItem(UINT);
};

extern CTopToolApp theApp;
