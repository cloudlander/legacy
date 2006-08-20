// NewDiskWizard.h : header file
//
// This class defines custom modal property sheet 
// CNewDiskWizard.
 
#ifndef __NEWDISKWIZARD_H__
#define __NEWDISKWIZARD_H__

#include "MyPropertyPage1.h"

/////////////////////////////////////////////////////////////////////////////
// CNewDiskWizard

class CNewDiskWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CNewDiskWizard)

// Construction
public:
	CNewDiskWizard(CWnd* pWndParent = NULL);

// Attributes
public:
	CNewFileName m_Page1;
	CNewDiskSize m_Page2;
	CNewBlockSize m_Page3;
	CNewFinish m_Page4;

	typedef struct Diskinfo{
		char file[255];  // Disk File Name
		int  DiskSize;   // Disk Size
		int  BlockSize;  // Block Size
	};
	Diskinfo info;
// Operations
public:
	int ChangeTag(int,char*);   // Change Show
	int Power(int);             // get 2*2*....
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDiskWizard)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewDiskWizard();

// Generated message map functions
protected:
	//{{AFX_MSG(CNewDiskWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __NEWDISKWIZARD_H__
