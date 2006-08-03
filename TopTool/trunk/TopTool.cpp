// TopTool.cpp : ����Ӧ�ó��������Ϊ��
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


// CTopToolApp ����

CTopToolApp::CTopToolApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTopToolApp ����

CTopToolApp theApp;


// CTopToolApp ��ʼ��

BOOL CTopToolApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
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
