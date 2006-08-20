// FSLogo.cpp : implementation file
//

#include "stdafx.h"
#include "FSWizard.h"
#include "FSLogo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFSLogo dialog


CFSLogo::CFSLogo(CWnd* pParent /*=NULL*/)
	: CDialog(CFSLogo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFSLogo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFSLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFSLogo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFSLogo, CDialog)
	//{{AFX_MSG_MAP(CFSLogo)
	//}}AFX_MSG_MAP
		ON_WM_TIMER()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFSLogo message handlers
void CFSLogo::OnTimer(UINT nIDEvent) 
{
	/*-------------------------------------
	 * Use following code.
	 *-------------------------------------*/
	static int temp = 0;
	temp++;
	if (temp > 150 )
	{
		temp = 0; 
		KillTimer(2);
//		ShowWindow(SW_HIDE);
		EndDialog(0);		 
	}
	CClientDC dc(this);
	CDC  dcMem;
	dcMem.CreateCompatibleDC(&dc);
	
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	dc.BitBlt(0,0,m_nWidth,m_nHeight,&dcMem,0,0,SRCCOPY);

}

BOOL CFSLogo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	::SetTimer(m_hWnd,2, 1, NULL);
		m_bitmap.LoadBitmap( IDB_BITMAP6 );
	
	BITMAP BitMap;
	m_bitmap.GetBitmap(&BitMap);
	m_nWidth = BitMap.bmWidth;
	m_nHeight = BitMap.bmHeight;
	
	SetWindowPos(NULL, 0,0,m_nWidth, m_nHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW);
	CenterWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
