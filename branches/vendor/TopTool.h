// TopTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
extern vector<WndElem> g_WndVec;
extern BOOL CALLBACK EnumProc(HWND hwnd,LPARAM lParam);

#include "resource.h"		// ������
#include "SystemTray.h"

// CTopToolApp:
// �йش����ʵ�֣������ TopTool.cpp
//

class CTopToolApp : public CWinApp
{
public:
	CSystemTray m_SysTrayWnd;
	CFrameWnd*	m_pTempWnd;
public:
	CTopToolApp();
	
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAbout();
	afx_msg void OnSelItem(UINT);
};

extern CTopToolApp theApp;
