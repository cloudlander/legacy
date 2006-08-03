#if !defined(AFX_EVENTDLG_H__62513724_FB99_43EF_8F95_DE70F125026E__INCLUDED_)
#define AFX_EVENTDLG_H__62513724_FB99_43EF_8F95_DE70F125026E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventDlg dialog

class CEventDlg : public CDialog
{
// Construction
public:
	void InsertString(CString s);
	CEventDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEventDlg)
	enum { IDD = IDD_DIALOG1 };
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventDlg)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	void OnCancel();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
	BOOL m_bMore;
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck2();
	BOOL m_bCapAll;
	afx_msg void OnLbnDblclkList1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTDLG_H__62513724_FB99_43EF_8F95_DE70F125026E__INCLUDED_)
