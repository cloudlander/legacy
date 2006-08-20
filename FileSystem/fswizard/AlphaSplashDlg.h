
////////////////////////////////////////////////
// CAlphaSplashDlg Class
//
// Name:		AlphaSplashDlg.h
//
// Author:		Rick
// Environment: Win98/NT/2000
// Date:		2001.12.27.
/////////////////////////////////////////////////


#if !defined(AFX_ALPHASPLASHDLG_H__80604581_B61F_11D2_A198_00902735233D__INCLUDED_)
#define AFX_ALPHASPLASHDLG_H__80604581_B61F_11D2_A198_00902735233D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAlphaSplashDlg dialog

class CAlphaSplashDlg : public CDialog
{
// Construction
public:
	CAlphaSplashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlphaSplashDlg)
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlphaSplashDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlphaSplashDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


/*---------------------------
 * Use this method & variable
 *---------------------------*/
// Variable
public:
	int m_nCount;			
	BLENDFUNCTION m_bf;			// AlphaBlend function parameter
	CBitmap  m_bitmap;			// Splash Bitmap
	int m_nWidth, m_nHeight;	// Splash Bitmap's width & height

// Method
public:
	void Create(CWnd *pParent);		
	void OnInitSplash();			// Initialize variable.
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALPHASPLASHDLG_H__80604581_B61F_11D2_A198_00902735233D__INCLUDED_)
