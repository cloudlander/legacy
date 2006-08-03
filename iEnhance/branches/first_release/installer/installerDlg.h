// installerDlg.h : header file
//

#if !defined(AFX_INSTALLERDLG_H__D1F52110_47FC_4D84_BC50_010888D71FF8__INCLUDED_)
#define AFX_INSTALLERDLG_H__D1F52110_47FC_4D84_BC50_010888D71FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInstallerDlg dialog

class CInstallerDlg : public CDialog
{
// Construction
public:
	CInstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInstallerDlg)
	enum { IDD = IDD_INSTALLER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInstallerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInstall();
	afx_msg void OnUninstall();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLERDLG_H__D1F52110_47FC_4D84_BC50_010888D71FF8__INCLUDED_)
