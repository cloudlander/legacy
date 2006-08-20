// SearchAvailDlg.h : header file
//

#if !defined(AFX_SEARCHAVAILDLG_H__1D60257B_CFA4_4289_8754_321C392A00E3__INCLUDED_)
#define AFX_SEARCHAVAILDLG_H__1D60257B_CFA4_4289_8754_321C392A00E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSearchAvailDlg dialog

class CSearchAvailDlg : public CDialog
{
// Construction
public:
	CSearchAvailDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchAvailDlg)
	enum { IDD = IDD_SEARCHAVAIL_DIALOG };
	CStatic	m_Status;
	UINT	m_Interval;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchAvailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSearchAvailDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHAVAILDLG_H__1D60257B_CFA4_4289_8754_321C392A00E3__INCLUDED_)
