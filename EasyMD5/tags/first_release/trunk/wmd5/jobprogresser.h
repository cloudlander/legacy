#if !defined(AFX_JOBPROGRESSER_H__A28331AD_40AE_4B92_9DCB_472D8D643B33__INCLUDED_)
#define AFX_JOBPROGRESSER_H__A28331AD_40AE_4B92_9DCB_472D8D643B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobProgresser.h : header file
//
/********************************************************************
	created:	2005/03/03
	created:	3:3:2005   22:04
	filename: 	h:\my_projects\easymd5\wmd5\jobprogresser.h
	file path:	h:\my_projects\easymd5\wmd5
	file base:	jobprogresser
	file ext:	h
	author:		Ronny(zhangxiaomin@gmail.com)
	
	purpose:	define two classes to do the customized progress updating
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CJobProgresser window

class CJobProgresser : public CProgressCtrl
{
// Construction
public:
	CJobProgresser();

// Attributes
public:
	CListCtrl* m_pJobList;		// point to dlg's listctrl
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobProgresser)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetNewPos(int i,int p);
	virtual ~CJobProgresser();

	// Generated message map functions
protected:
	int m_nPos;
	//{{AFX_MSG(CJobProgresser)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CQueueProgresser window

class CQueueProgresser : public CProgressCtrl
{
// Construction
public:
	CQueueProgresser();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueueProgresser)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CQueueProgresser();

	// Generated message map functions
protected:
	//{{AFX_MSG(CQueueProgresser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBPROGRESSER_H__A28331AD_40AE_4B92_9DCB_472D8D643B33__INCLUDED_)
