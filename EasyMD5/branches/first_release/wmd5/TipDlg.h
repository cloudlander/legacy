#if !defined(AFX_TIPDLG_H__9FFBED38_DCDE_4445_A7E7_73054B2CB8F8__INCLUDED_)
#define AFX_TIPDLG_H__9FFBED38_DCDE_4445_A7E7_73054B2CB8F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipDlg dialog

class CTipDlg : public CDialog
{
// Construction
public:
	CTipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTipDlg)
	enum { IDD = IDD_TIPBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPDLG_H__9FFBED38_DCDE_4445_A7E7_73054B2CB8F8__INCLUDED_)
