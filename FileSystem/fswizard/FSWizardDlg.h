// FSWizardDlg.h : header file
//

#if !defined(AFX_FSWIZARDDLG_H__4D8BE94A_DE09_4BA2_8306_022848D107D1__INCLUDED_)
#define AFX_FSWIZARDDLG_H__4D8BE94A_DE09_4BA2_8306_022848D107D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FSlogo.h"
/////////////////////////////////////////////////////////////////////////////
// CFSWizardDlg dialog

class CFSWizardDlg : public CDialog
{
// Construction
public:

	void OnWizard();
	CFSWizardDlg(CWnd* pParent = NULL);	// standard constructor
	int SearchVDK();
	int IsFormated(char*,int&,int&,int&);
	int Format(char*,int,int);

	CFSLogo* m_pSplash;	
// Dialog Data
	//{{AFX_DATA(CFSWizardDlg)
	enum { IDD = IDD_FSWIZARD_DIALOG };
	CListBox	m_DiskList;
	CString	m_Res;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFSWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFSWizardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNew();
	afx_msg void OnStart();
	afx_msg void OnDblclkDiskList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSWIZARDDLG_H__4D8BE94A_DE09_4BA2_8306_022848D107D1__INCLUDED_)
