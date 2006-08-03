#if !defined(AFX_TIPWND_H__AFA2DCF4_10EF_495E_AAC1_07E0AFE63BF2__INCLUDED_)
#define AFX_TIPWND_H__AFA2DCF4_10EF_495E_AAC1_07E0AFE63BF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TipWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipWnd window

class CTipWnd : public CWnd
{
// Construction
public:
	CTipWnd();

// Attributes
public:
	CString str;
	CRect	rc;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetStaticText(char* s);
	virtual ~CTipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTipWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPWND_H__AFA2DCF4_10EF_495E_AAC1_07E0AFE63BF2__INCLUDED_)
