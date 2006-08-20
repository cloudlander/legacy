// FSWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FSWizard.h"
#include "FSWizardDlg.h"
#include "NewDiskWizard.h"
#include <fstream.h>
#include <io.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFSWizardDlg dialog

CFSWizardDlg::CFSWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFSWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFSWizardDlg)
	m_Res = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFSWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFSWizardDlg)
	DDX_Control(pDX, IDC_LIST1, m_DiskList);
	DDX_Text(pDX, IDC_RESULT, m_Res);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFSWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CFSWizardDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(ID_START, OnStart)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkDiskList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFSWizardDlg message handlers

BOOL CFSWizardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	SearchVDK();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFSWizardDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFSWizardDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFSWizardDlg::OnWizard()
{
	// TODO: The property sheet attached to your project
	// via this function is not hooked up to any message
	// handler.  In order to actually use the property sheet,
	// you will need to associate this function with a control
	// in your project such as a menu item or tool bar button.
	int nRet;
	CNewDiskWizard myWizard;
//	myWizard.m_psh.dwFlags &= ~(PSH_HASHELP);
	nRet=myWizard.DoModal();
	
	switch ( nRet )
	{
	case -1: 
		AfxMessageBox("Dialog box could not be created!");
		break;
	case IDABORT:
		// Do something.
		break;
	case IDOK:
		SearchVDK();
		break;
	case IDCANCEL:
		// Do something.
		break;
	default:
		// Do something.
		break;
	};
}

void CFSWizardDlg::OnNew() 
{
	OnWizard();	
}

int CFSWizardDlg::SearchVDK()
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	
	BOOL fFinished = FALSE; 
	m_DiskList.ResetContent();
	
	// Start searching for .VDK files in the current directory. 
	
	hSearch = FindFirstFile("*.vdk", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		m_Res.Format("No Avaliable Disks found");	
		UpdateData(FALSE);
		
		return 1;
	} 
	
	
	while (!fFinished) 
	{ 
		m_DiskList.AddString(FileData.cFileName);
		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
				fFinished = TRUE; 
		}
	} 
	
	// Close the search handle. 
	
	if (!FindClose(hSearch)) 
	{ 
		AfxMessageBox("Fatal Error, Please check your settings");  
		EndDialog(1);
		return -1; 
	} 

	m_Res.Format("Avaliable Disks found here");	
	UpdateData(FALSE);
	return 0;	
}

void CFSWizardDlg::OnStart() 
{
	//  磁盘文件第一个字节是magic number（0xF7）
	//  第二个字节为格式化标志
	//  0x00: formated  0x01:  not formated  
	//  3,4自己存放块大小 (永远存在)
	//  5,6,7,8字节存放文件大小(format 后消失)
	//  该数据由FSNewDiskWizard产生
	//  创建根目录由FS.exe完成
	//  创建成功标志为第2个字节的第一位为1
	//  磁盘clean位为第2个字节的第二位
	int DiskSize,FATBlock,BlockSize;
	char file[100];
	int i=m_DiskList.GetCurSel();
	if( LB_ERR == i){
		AfxMessageBox("No Disk file Selected");
		return ;
	}
	else
		m_DiskList.GetText(i,file);
	int res=IsFormated(file,DiskSize,BlockSize,FATBlock);
	if( 0 == res)
	{
		// prompt for format  :: MessageBox
		if(IDYES == AfxMessageBox("Not Formated\nDo you want to format it now?",MB_YESNO)){
			Format(file,DiskSize,FATBlock);
		}
		else
			return;
		char str[256];
		sprintf(str,"fs.exe %s %d %d %d",file,DiskSize,BlockSize,FATBlock);
		STARTUPINFO sp={1};
		PROCESS_INFORMATION pi;
		m_pSplash = new CFSLogo;
		m_pSplash->DoModal();

		if( FALSE == CreateProcess(NULL,str,NULL,NULL,0,0,NULL,NULL,&sp,&pi)){
			AfxMessageBox("Fatal Error: Can't find FS.exe");
			delete m_pSplash;
			return ;
		}
		delete m_pSplash;
		EndDialog(0);
	}
	else if( 1 == res || 2 == res)
	{
		// start New Process of File System
		char str[256];
		sprintf(str,"fs.exe %s %d %d %d",file,DiskSize,BlockSize,FATBlock);
//		AfxMessageBox(str);
		STARTUPINFO sp={1};
		PROCESS_INFORMATION pi;

		m_pSplash = new CFSLogo;
		m_pSplash->DoModal();
		
		if( FALSE == CreateProcess(NULL,str,NULL,NULL,0,0,NULL,NULL,&sp,&pi)){
			AfxMessageBox("Fatal Error: Can't find FS.exe");
			delete m_pSplash;
			return;
		}
		delete m_pSplash;
		EndDialog(0);
	}else if( 3 == res)
	{
		// disk not supported
		AfxMessageBox("Disk Not Supported");
	}
	else{
		// prompt for error   :: interl error
		AfxMessageBox("Fatal Error!\nPlease check that your disk file is not modified out of this program");
		EndDialog(1);
	}
	
	
}

int CFSWizardDlg::IsFormated(char* filename,int& DiskSize,int& BlockSize,int& FATBlock)
{
	fstream disk;
	disk.open(filename,ios::in | ios::binary | ios::nocreate);
	
	char byte;
	disk.seekp(ios::beg);
	disk.read(&byte,1);
	unsigned char temp[4];
	if( 0xF7 == (unsigned char)byte )  // Magic Number
	{
		disk.read(&byte,1);
		if( 0x01 == (unsigned char)byte )  // not formated
		{
			BlockSize=0;
			disk.read(temp,2);
			memcpy(&BlockSize,temp,2);
			disk.read(temp,4);
			memcpy(&DiskSize,temp,4);
			FATBlock=DiskSize/BlockSize;
			FATBlock=2*FATBlock / BlockSize;
			disk.close();
			return 0;
		}
		else if ( 0x80 == (unsigned char)byte || 0xC0 == (unsigned char)byte) // root dir has been created
		{
			BlockSize=0;
			disk.read(temp,2);
			memcpy(&BlockSize,temp,2);
			long fd=disk.fd();
			DiskSize=_filelength(fd);
			FATBlock=DiskSize/BlockSize;
			FATBlock=2*FATBlock / BlockSize;
			disk.close();
			return 1;
		}
		else if ( 0x00 == (unsigned char)byte ){ // root dir not created
			BlockSize=0;
			disk.read(temp,2);
			memcpy(&BlockSize,temp,2);
			long fd=disk.fd();
			DiskSize=_filelength(fd);
			FATBlock=DiskSize/BlockSize;
			FATBlock=2*FATBlock / BlockSize;
			disk.close();
			return 2;      
			}
			else{
				disk.close();  // fatal error
				return -1;
			}
	}
	else{
		disk.close();
		return 3;           // disk not support
	}
}

int CFSWizardDlg::Format(char* filename,int DiskSize,int FATBlock)
{
	fstream disk;
	disk.open(filename,ios::in | ios::out | ios::binary | ios::nocreate);
	unsigned char Temp=0;
	disk.seekp(4,ios::beg);
	for (int i=4; i<DiskSize;i++)
	{
		if (disk.write(&Temp,1)==NULL)
		{
			return 1;
		}
		//*/
	}
	// 写FAT
	Temp=255;
    disk.seekp(4,ios::beg);
	for (i=4;i<2*FATBlock;i++)  //每个块号项占2Byte
	{
		if (disk.write(&Temp,1)==NULL)
			return 1;
	}
    // 写format标志位,但并没有创建根目录
	disk.seekp(0,ios::beg);
	Temp=0xF7;
	if (disk.write(&Temp,1)==NULL)
		return 1;
	Temp=0x00;
	if (disk.write(&Temp,1)==NULL)
		return 1;
	disk.close();
	return 0;
	
}



void CFSWizardDlg::OnDblclkDiskList() 
{
	// TODO: Add your control notification handler code here
	OnStart();
}
