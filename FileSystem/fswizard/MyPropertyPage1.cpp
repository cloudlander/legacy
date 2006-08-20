// MyPropertyPage1.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyPropertyPage1.h"
#include "NewDiskWizard.h"
#include <fstream.h>
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CNewFileName, CPropertyPage)
IMPLEMENT_DYNCREATE(CNewDiskSize, CPropertyPage)
IMPLEMENT_DYNCREATE(CNewBlockSize, CPropertyPage)
IMPLEMENT_DYNCREATE(CNewFinish, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CNewFileName property page

CNewFileName::CNewFileName() : CPropertyPage(CNewFileName::IDD)
{
	//{{AFX_DATA_INIT(CNewFileName)
	m_DiskName = _T("");
	//}}AFX_DATA_INIT
}

CNewFileName::~CNewFileName()
{
}

void CNewFileName::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFileName)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_DiskName);
	DDV_MaxChars(pDX, m_DiskName, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFileName, CPropertyPage)
	//{{AFX_MSG_MAP(CNewFileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewDiskSize property page

CNewDiskSize::CNewDiskSize() : CPropertyPage(CNewDiskSize::IDD)
{
	//{{AFX_DATA_INIT(CNewDiskSize)
	m_Size = _T("");
	temp=24;
	
	//}}AFX_DATA_INIT
}

CNewDiskSize::~CNewDiskSize()
{
}

void CNewDiskSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDiskSize)
	DDX_Control(pDX, IDC_SLIDER1, m_DiskSize);
	DDX_Text(pDX, IDC_DISKSIZE, m_Size);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewDiskSize, CPropertyPage)
	//{{AFX_MSG_MAP(CNewDiskSize)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewBlockSize property page

CNewBlockSize::CNewBlockSize() : CPropertyPage(CNewBlockSize::IDD)
{
	//{{AFX_DATA_INIT(CNewBlockSize)
	m_BlockSize = _T("");
	temp=0;
	//}}AFX_DATA_INIT
}

CNewBlockSize::~CNewBlockSize()
{
}

void CNewBlockSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewBlockSize)
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Text(pDX, IDC_BLOCKSIZE, m_BlockSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewBlockSize, CPropertyPage)
	//{{AFX_MSG_MAP(CNewBlockSize)
	ON_WM_HSCROLL()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewFinish property page

CNewFinish::CNewFinish() : CPropertyPage(CNewFinish::IDD)
{
	//{{AFX_DATA_INIT(CNewFinish)
	m_Finish = _T("");
	//}}AFX_DATA_INIT
}

CNewFinish::~CNewFinish()
{
}

void CNewFinish::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFinish)
	DDX_Text(pDX, IDC_FINISH, m_Finish);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFinish, CPropertyPage)
	//{{AFX_MSG_MAP(CNewFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



LRESULT CNewFileName::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	if( m_DiskName == ""){  // Disk File can't be empty
		AfxMessageBox("Error:  Please specify your new disk file name");
		m_edit.SetFocus();
		return -1;
	}

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 

	m_DiskName.Insert(m_DiskName.GetLength(),".VDK");

	hSearch = FindFirstFile(m_DiskName, &FileData); 
	if (GetLastError()==ERROR_FILE_NOT_FOUND)
	{	
		strcpy(pWizard->info.file,m_DiskName);
		return CPropertyPage::OnWizardNext();
	}else{
		if( IDYES == AfxMessageBox("Warning: Overwrite existing disk file?",MB_YESNO)){
			if (!FindClose(hSearch)) 
			{ 	
				AfxMessageBox("Fatal Error, Please check your settings");  
				EndDialog(1);
				return -1; 
			} 
			strcpy(pWizard->info.file,m_DiskName);
			return CPropertyPage::OnWizardNext();
		}
		else{
			m_edit.SetFocus();
			return -1;
		}
	}
		
	
}



BOOL CNewDiskSize::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);
	m_DiskSize.SetRange(17,31);
	char str[255];
	m_DiskSize.SetPos(24);
	pWizard->ChangeTag(24,str);
	m_Size.Format(str);
	UpdateData(FALSE);	// TODO: Add extra initialization here
	m_psp.dwFlags &= ~(PSH_HASHELP);
	
	return CPropertyPage::OnSetActive();
}

void CNewDiskSize::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}

void CNewDiskSize::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	char str[100];


	if(nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK){
		pWizard->ChangeTag(nPos,str);
		m_Size.Format(str);
		UpdateData(FALSE);
		temp=nPos;
	}
	
	pWizard->info.DiskSize=temp;	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CNewBlockSize::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	char str[100];
	// Get the minimum and maximum scroll-bar positions.
/*
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();
	
	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:    // Scroll to far left.
		curpos = minpos;
		break;
		
	case SB_RIGHT:    // Scroll to far right.
		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:  // End scroll.
		break;
		
	case SB_LINELEFT:    // Scroll left.
		if (curpos > minpos)
			curpos--;
		break;
		
	case SB_LINERIGHT:  // Scroll right.
		if (curpos < maxpos)
			curpos++;
		break;
		
	case SB_PAGELEFT:   // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO  info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:    // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO  info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			
			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag 
		// operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is 
		// the
		curpos = nPos;     // position that the scroll box has been dragged 
		// to.
		break;
	}
	
	// Set the new position of the thumb (scroll box).

	pScrollBar->SetScrollPos(curpos);
*/
	if(nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK){
		pWizard->ChangeTag(nPos,str);
		m_BlockSize.Format(str);
		UpdateData(FALSE);
		temp=nPos;
	}
	
	pWizard->info.BlockSize=temp;	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CNewBlockSize::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

BOOL CNewBlockSize::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);
	
	int DS=(pWizard->info.DiskSize)/2;
	temp=DS;

	m_Slider.SetRange(8,DS);
	m_Slider.SetPos((8+DS)/2);
	char str[255];
	pWizard->ChangeTag((8+DS)/2,str);
	m_BlockSize.Format(str);
	UpdateData(FALSE);
	m_psp.dwFlags &= ~(PSH_HASHELP);
	
	return CPropertyPage::OnSetActive();
}

LRESULT CNewBlockSize::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->info.BlockSize=temp;
	return CPropertyPage::OnWizardNext();
}

LRESULT CNewDiskSize::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->info.DiskSize=temp;
	return CPropertyPage::OnWizardNext();
}



BOOL CNewFinish::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	char str[1000];
	char str1[1000];
	char str2[200];
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	sprintf(str,"Your Disk Name is %s\n\n\n",pWizard->info.file);

	pWizard->ChangeTag(pWizard->info.DiskSize,str2);
	sprintf(str1,"Your Disk Size is %s\n\n\n",str2);
	strcat(str,str1);

	pWizard->ChangeTag(pWizard->info.BlockSize,str2);
	sprintf(str1,"The Block Size of Your Disk is %s",str2);
	strcat(str,str1);

	m_Finish.Format(str);
	UpdateData(FALSE);
	
	return CPropertyPage::OnSetActive();
}

BOOL CNewFileName::OnSetActive() 
{
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	pWizard->SetWizardButtons(PSWIZB_NEXT);
	CButton* pcb=(CButton*)GetParent()->GetDlgItem(IDHELP);
	HWND hwnd=pcb->GetSafeHwnd();
	::ShowWindow(hwnd,SW_HIDE);
	// TODO: Add your specialized code here and/or call the base class
	return CPropertyPage::OnSetActive();
}

BOOL CNewFinish::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	fstream fs;
	CNewDiskWizard* pWizard=(CNewDiskWizard*)GetParent();
	char file[256];
	char* data;
	strcpy(file,pWizard->info.file);
	fs.open(file,ios::out | ios::binary );
	data=new char[1];
	*data=0xF7;		// magic number
	fs.seekp(ios::beg);
	fs.write(data,1);
	*data=0x01;		// not formated
	fs.write(data,1);
	delete[] data;
	data=new char[2];
	int i=pWizard->Power(pWizard->info.BlockSize);
	memcpy(data,&i,2);
	fs.write(data,2);
	delete[] data;
	data=new char[4];
	i=pWizard->Power(pWizard->info.DiskSize);
	memcpy(data,&i,4);
	fs.write(data,4);
	delete[] data;
	fs.close();
	EndDialog(IDOK);
	return CPropertyPage::OnWizardFinish();
}



BOOL CNewFileName::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CButton* pcb=(CButton*)GetDlgItem(IDHELP);
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

