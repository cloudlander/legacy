// MyPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NewDiskWizard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDiskWizard

IMPLEMENT_DYNAMIC(CNewDiskWizard, CPropertySheet)

CNewDiskWizard::CNewDiskWizard(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);

	SetWizardMode();
}

CNewDiskWizard::~CNewDiskWizard()
{
}


BEGIN_MESSAGE_MAP(CNewDiskWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CNewDiskWizard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CNewDiskWizard::ChangeTag(int n,char* str)
{
	if(n>=30)
		sprintf(str,"%d G",Power(n-30));
	else if(n>=20)
		sprintf(str,"%d M",Power(n-20));
	else if(n>=10)
		sprintf(str,"%d K",Power(n-10));
	else
		sprintf(str,"%d Byte",Power(n));
	return 0;
}

int CNewDiskWizard::Power(int n)
{
	int sum=1;
	if(n>0){
		for(;n>0;n--)
			sum=sum*2;
	}
	return sum;
}
/////////////////////////////////////////////////////////////////////////////
// CNewDiskWizard message handlers


