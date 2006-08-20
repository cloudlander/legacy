// FTPClientDlg.h : header file
//

#if !defined(AFX_FTPCLIENTDLG_H__B02B8026_7EA2_492D_838B_66C3E9DFD6A8__INCLUDED_)
#define AFX_FTPCLIENTDLG_H__B02B8026_7EA2_492D_838B_66C3E9DFD6A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FTPClient.h"

class CSystemTray;

/////////////////////////////////////////////////////////////////////////////
// CFTPClientDlg dialog

class CFTPClientDlg : public CDialog
{
// Construction
public:
	CFTPClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFTPClientDlg)
	enum { IDD = IDD_FTPCLIENT_DIALOG };
	CIPAddressCtrl	m_IPFtp;
	CIPAddressCtrl	m_IPAuthSvr;
	CString	m_AuthPwd;
	CString	m_AuthUsr;
	CString	m_FtpPwd;
	CString	m_FtpUsr;
	UINT	m_Interval;
	CString	m_Path;
	BOOL	m_NeedLog;
	BOOL	m_NeedLoop;
	BOOL	m_NeedUser;
	UINT	m_Port;
	//}}AFX_DATA

	BOOL m_bFirstRun;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPClientDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFTPClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSTRestore();
	afx_msg void OnSTExit();
	afx_msg void OnStart();
	afx_msg void OnNeeduser();
	afx_msg void OnNeedlog();
	afx_msg void OnNeedloop();
	afx_msg void OnBrowse();
	afx_msg void OnAuthenticate();
    //}}AFX_MSG(CFTPClientDlg)

	DECLARE_MESSAGE_MAP()
	CWinThread *th;
	
	//// Internal support functions
	void SetupTrayIcon();
	void SetupTaskBarButton();

	//// Internal data
	bool bMinimized_;
	CSystemTray* pTrayIcon_;
	int nTrayNotificationMsg_;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPCLIENTDLG_H__B02B8026_7EA2_492D_838B_66C3E9DFD6A8__INCLUDED_)
