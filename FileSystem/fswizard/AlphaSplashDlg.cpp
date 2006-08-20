
////////////////////////////////////////
// CAlphaSplashDlg Class 
//
// Name:		AlphaSplashDlg.cpp
// Author:		Rick
// Environment: Win98/NT/2000
// Date:		2001.12.28.
////////////////////////////////////////
 
 

#include "stdafx.h"
#include "AlphaSplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Include this file & insert 'msimg32.lib' into your project!!
#include <wingdi.h>


/////////////////////////////////////////////////////////////////////////////
// CAlphaSplashDlg dialog


CAlphaSplashDlg::CAlphaSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlphaSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlphaSplashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAlphaSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlphaSplashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlphaSplashDlg, CDialog)
	//{{AFX_MSG_MAP(CAlphaSplashDlg)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlphaSplashDlg message handlers




//*****************************************************************************
// CAlphaSplashDlg::OnTimer()
// Purpose: [ Show your Splash Bitmap by growing 'SourceConstantAlpha' value. ]
// Parameters:	
// [nIDEvent] : Timer ID.
// Returns:	none
//*****************************************************************************
void CAlphaSplashDlg::OnTimer(UINT nIDEvent) 
{
	/*-------------------------------------
	 * Use following code.
	 *-------------------------------------*/
	static int temp = 0;

	if (temp > 255 - 10)
	{
		temp = 0; 
		KillTimer(1);
//		ShowWindow(SW_HIDE);
		EndDialog(0);		 
	}
	else
	{
		m_nCount += 100;				// Step length for alpha render	
		temp += (m_nCount/2000);	// Display step length
		m_bf.SourceConstantAlpha = temp + 10;

		CClientDC dc(this);
		CDC  dcMem;
		dcMem.CreateCompatibleDC(&dc);
		
		CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
		AlphaBlend(dc, 0,0, m_nWidth, m_nHeight, dcMem, 0,0,m_nWidth, m_nHeight,m_bf); 
		
		dcMem.SelectObject(pOldBitmap);
	}
	
	//////////////////////////////////////////////////////////////

	CDialog::OnTimer(nIDEvent);
}



//*****************************************************************************
// CAlphaSplashDlg::OnInitDialog()
// Purpose: [ Load Splash Bitmap & Initialize BLENDFUNCTION structure. ]
// Parameters:	
// Returns:	BOOL
//*****************************************************************************
BOOL CAlphaSplashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/*--------------------
	 * Use following code.
	 *--------------------*/
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.SourceConstantAlpha = 0;
	m_bf.AlphaFormat = 0;

	m_bitmap.LoadBitmap(IDB_SPLASH);
	
	BITMAP BitMap;
	m_bitmap.GetBitmap(&BitMap);
	m_nWidth = BitMap.bmWidth;
	m_nHeight = BitMap.bmHeight;
	
	SetWindowPos(NULL, 0,0,m_nWidth, m_nHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW);

	m_nCount = 0;
	CenterWindow();
	
	OnInitSplash();
	////////////////////////////

	return TRUE;  
}


//*****************************************************************************
// CAlphaSplashDlg::OnEraseBkgnd()
// Purpose: [ Flicker Free Dialog.. :) ]
// Parameters:	
// [pDC] : CDC pointer
// Returns:	BOOL
//*****************************************************************************
BOOL CAlphaSplashDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


//*****************************************************************************
// CAlphaSplashDlg::Create()
// Purpose: [ To make Modeless Dialog.. :) ]
// Parameters:	
// [*pParent] : Parent Window's pointer
// Returns:	none
//*****************************************************************************
void CAlphaSplashDlg::Create(CWnd *pParent)
{
	CDialog::Create(CAlphaSplashDlg::IDD, pParent);
}


//*****************************************************************************
// CAlphaSplashDlg::OnInitSplash()
// Purpose: [ Initialize variable & set timer.]
// Parameters:	
// Returns:	none
//*****************************************************************************
void CAlphaSplashDlg::OnInitSplash()
{
	m_nCount = 0;
	::SetTimer(m_hWnd, 1, 1, NULL);
}

