// MyPropertyPage1.h : header file
//

#ifndef __MYPROPERTYPAGE1_H__
#define __MYPROPERTYPAGE1_H__

/////////////////////////////////////////////////////////////////////////////
// CNewFileName dialog

class CNewFileName : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewFileName)

// Construction
public:
	CNewFileName();
	~CNewFileName();

// Dialog Data
	//{{AFX_DATA(CNewFileName)
	enum { IDD = IDD_PROPPAGE1 };
	CEdit	m_edit;
	CString	m_DiskName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewFileName)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewFileName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CNewDiskSize dialog

class CNewDiskSize : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewDiskSize)

// Construction
public:
	CNewDiskSize();
	~CNewDiskSize();
	int temp;
// Dialog Data
	//{{AFX_DATA(CNewDiskSize)
	enum { IDD = IDD_PROPPAGE2 };
	CSliderCtrl	m_DiskSize;
	CString	m_Size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewDiskSize)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewDiskSize)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CNewBlockSize dialog

class CNewBlockSize : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewBlockSize)

// Construction
public:
	CNewBlockSize();
	~CNewBlockSize();
	int temp;
// Dialog Data
	//{{AFX_DATA(CNewBlockSize)
	enum { IDD = IDD_PROPPAGE3 };
	CSliderCtrl	m_Slider;
	CString	m_BlockSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewBlockSize)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewBlockSize)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CNewFinish dialog

class CNewFinish : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewFinish)

// Construction
public:
	CNewFinish();
	~CNewFinish();

// Dialog Data
	//{{AFX_DATA(CNewFinish)
	enum { IDD = IDD_PROPPAGE4 };
	CString	m_Finish;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewFinish)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewFinish)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __MYPROPERTYPAGE1_H__
