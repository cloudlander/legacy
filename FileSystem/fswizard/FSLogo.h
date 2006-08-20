#if !defined(AFX_FSLOGO_H__C1FED7A4_0F10_40B7_A80B_110F74893C73__INCLUDED_)
#define AFX_FSLOGO_H__C1FED7A4_0F10_40B7_A80B_110F74893C73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FSLogo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFSLogo dialog

class CFSLogo : public CDialog
{
// Construction
public:
	CFSLogo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFSLogo)
	enum { IDD = IDD_FSSTARTLOGO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	afx_msg void OnTimer(UINT nIDEvent);
	
	// Variable
public:
	CBitmap  m_bitmap;			// Splash Bitmap
	int m_nWidth, m_nHeight;	// Splash Bitmap's width & height
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFSLogo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFSLogo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSLOGO_H__C1FED7A4_0F10_40B7_A80B_110F74893C73__INCLUDED_)
