#if !defined(AFX_GPADLG_H__1704FDC6_AA84_4698_9BCF_96C85453E214__INCLUDED_)
#define AFX_GPADLG_H__1704FDC6_AA84_4698_9BCF_96C85453E214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GpaDlg.h : header file
//
#include <process.h>
#include "TipWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CGpaDlg dialog

class CGpaDlg : public CDialog
{
// Construction
public:
	CGpaDlg(CWnd* pParent = NULL);   // standard constructor
	~CGpaDlg()
	{
/*		if(m_pTipWnd)
		{
			delete m_pTipWnd;
		}
*/
	}
private:

// Dialog Data
	//{{AFX_DATA(CGpaDlg)
	enum { IDD = IDD_SCOREDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGpaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPADLG_H__1704FDC6_AA84_4698_9BCF_96C85453E214__INCLUDED_)
