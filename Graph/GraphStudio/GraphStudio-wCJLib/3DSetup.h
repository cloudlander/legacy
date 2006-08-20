#if !defined(AFX_3DSETUP_H__EB316370_71A0_4C13_9269_DE278A1E6B5F__INCLUDED_)
#define AFX_3DSETUP_H__EB316370_71A0_4C13_9269_DE278A1E6B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DSetup dialog

class C3DSetup : public CDialog
{
// Construction
public:
	C3DSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3DSetup)
	enum { IDD = IDD_3DSETUP };
	CSliderCtrl	m_ctlVUPz;
	CSliderCtrl	m_ctlVUPy;
	CSliderCtrl	m_ctlVUPx;
	CSliderCtrl	m_ctlVPNz;
	CSliderCtrl	m_ctlVPNy;
	CSliderCtrl	m_ctlVPNx;
	CSliderCtrl	m_ctlCOPy;
	CSliderCtrl	m_ctlCOPz;
	CSliderCtrl	m_ctlCOPx;
	//}}AFX_DATA

	void SetView(CView* pView);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CView*		m_pView;
	// Generated message map functions
	//{{AFX_MSG(C3DSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DSETUP_H__EB316370_71A0_4C13_9269_DE278A1E6B5F__INCLUDED_)
