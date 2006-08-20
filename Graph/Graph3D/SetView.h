#if !defined(AFX_SETVIEW_H__80F544C7_F00A_4D37_9830_874605CDEE07__INCLUDED_)
#define AFX_SETVIEW_H__80F544C7_F00A_4D37_9830_874605CDEE07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSetView : public CFormView
{
protected:
	CSetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSetView)

// Form Data
public:
	//{{AFX_DATA(CSetView)
	enum { IDD = IDD_DIALOG1 };
	CSliderCtrl	m_ctlVUPz;
	CSliderCtrl	m_ctlVUPy;
	CSliderCtrl	m_ctlVUPx;
	CSliderCtrl	m_ctlVPNz;
	CSliderCtrl	m_ctlVPNy;
	CSliderCtrl	m_ctlVPNx;
	CSliderCtrl	m_ctlCOPz;
	CSliderCtrl	m_ctlCOPy;
	CSliderCtrl	m_ctlCOPx;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSetView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETVIEW_H__80F544C7_F00A_4D37_9830_874605CDEE07__INCLUDED_)
