// wmd5Dlg.h : header file
//

#if !defined(AFX_WMD5DLG_H__12F7361B_65BC_45CB_9B89_2A552ABCF1D3__INCLUDED_)
#define AFX_WMD5DLG_H__12F7361B_65BC_45CB_9B89_2A552ABCF1D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWmd5Dlg dialog

class CWmd5Dlg : public CDialog
{
// Construction
public:
	void ShowResult(CString total,CString error,CString match,CString notmatch);
	void ShowResult(CString total,CString error);
	CWorkQueue* m_pQueue;
	CWmd5Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWmd5Dlg)
	enum { IDD = IDD_WMD5_DIALOG };
	CStatic	m_success;
	CStatic	m_error;
	CStatic	m_notmatch;
	CStatic	m_match;
	CStatic	m_TimeLeft;
	CQueueProgresser	m_QueueProgress;
	CListCtrl	m_JobList;
	CString	m_JobPath;
	//}}AFX_DATA
	
#ifdef SHOW_TIME_LEFT
	CString	m_strTimeLeft;
#endif

//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWmd5Dlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

#ifdef SHOW_TIME_LEFT	
	afx_msg void OnTimer(UINT nIDEvent);
#endif

	// Generated message map functions
	//{{AFX_MSG(CWmd5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnClickJoblist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMD5DLG_H__12F7361B_65BC_45CB_9B89_2A552ABCF1D3__INCLUDED_)
