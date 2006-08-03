// TopTool.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TopTool.h"
#include "TopToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTopToolApp

BEGIN_MESSAGE_MAP(CTopToolApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND_RANGE(ID_MENUITEM_BASE,ID_MENUITEM_BASE+100,OnSelItem)
END_MESSAGE_MAP()


// CTopToolApp 构造

CTopToolApp::CTopToolApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTopToolApp 对象

CTopToolApp theApp;


// CTopToolApp 初始化

BOOL CTopToolApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();
	m_pTempWnd = new CFrameWnd();
	m_pTempWnd->LoadFrame(IDR_POPUP_MENU);
	m_pMainWnd = m_pTempWnd;
	if (!m_SysTrayWnd.Create(NULL,                          // Parent window
                           2001,							// Icon notify message to use
                           _T("TopTool!"),					// tooltip
                           LoadIcon(IDR_ICON),  // Icon to use
                           IDR_POPUP_MENU))                 // ID of tray icon
		return -1;
	m_SysTrayWnd.SetMenuDefaultItem(IDM_ABOUT, FALSE);
	return TRUE;
}

void CTopToolApp::OnAbout()
{
	// TODO: Add your command handler code here
	CTopToolDlg dlg;
	dlg.DoModal();
}

void CTopToolApp::OnSelItem(UINT item)
{
	item = item - ID_MENUITEM_BASE;
//	int i;
	LONG style;
	if(!g_WndVec[item].bTopmost)
	{
		style = ::GetWindowLong(g_WndVec[item].hwnd,GWL_EXSTYLE);
		::SetWindowLong(g_WndVec[item].hwnd,GWL_EXSTYLE,style|WS_EX_TOPMOST);
		::SetWindowPos(g_WndVec[item].hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED );
//		i=::SendMessage(g_WndVec[item].hwnd,0x00000147,0x00000000,0x00000000);
		::SetWindowPos(g_WndVec[item].hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
//		i=::SendMessage(g_WndVec[item].hwnd,0x0000014E,0xFFFFFFFF,0x00000000);
	}
	else
	{
		::SetWindowPos(g_WndVec[item].hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE );
	}
}
